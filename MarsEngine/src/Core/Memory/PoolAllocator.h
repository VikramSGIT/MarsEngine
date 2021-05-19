#ifndef POOLALLOCATOR
#define POOLALLOCATOR

#define ME_BUCKETSIZE 8 // make sure it to be powers of 2
#define ME_BUCKETCOUNT 20

#include "MemoryManager.h"
namespace ME
{

	template<size_t bytes>
	union Bucket
	{
		char item[bytes];
		Bucket* next;
	};

	typedef Bucket<ME_BUCKETSIZE> bucket;

	template<typename upstreammemory = alloc_dealloc_UpstreamMemory>
	class PoolAllocator : public MemoryManager
	{
	public:
		PoolAllocator()
			:Size(ME_BUCKETSIZE), Count(ME_BUCKETCOUNT), MemoryManager(new upstreammemory),
			m_PoolCount(1)
		{
			m_PoolHead = (bucket*)m_UpstreamMemory->allocate(sizeof(bucket) * ME_BUCKETCOUNT, "POOLALLOCATOR: Pool Initialization");
			m_Pools = (bucket**)m_UpstreamMemory->allocate(sizeof(bucket**), "POOLALLOCATOR: Ledger Initialization");
			new (m_Pools) bucket* (m_PoolHead);

			m_nextFree = m_PoolHead;

			for (size_t i = 1; i < ME_BUCKETCOUNT; i++)
				(m_PoolHead + (i - 1))->next = (m_PoolHead + i);
			(m_PoolHead + ME_BUCKETCOUNT - 1)->next = nullptr;
		}

		~PoolAllocator() 
		{
			for (size_t i = 0; i < m_PoolCount; i++)
				m_UpstreamMemory->deallocate(*(m_Pools + i), sizeof(bucket) * ME_BUCKETCOUNT, "POOLALLOCATOR: Deallocating pool");
			m_UpstreamMemory->deallocate((bucket*)m_Pools, sizeof(bucket*) * m_PoolCount, "POOLALLOCATOR: Deallocating pool ledger");
		}

		virtual void* allocate(const size_t& size = ME_BUCKETSIZE) override
		{

			if (size > ME_BUCKETSIZE * ME_BUCKETCOUNT)
				return nullptr;

			size_t continious = 0;
			bucket* cur = m_nextFree;

			// To find a contiguous pool of legnth "size"
			while (cur != nullptr)
			{
				bucket* temp = nullptr;
				if(cur->next != nullptr)
					temp = cur->next; // needed a fix here

				if (temp - cur == 1)
					continious++;
				else
					continious = 0;
				if (continious * sizeof(bucket) >= size)
				{
					m_nextFree = temp;
					return reinterpret_cast<void*>(temp - continious);
				}
				cur = temp;
			}

			// Pool Expantion
			bucket* expand = (bucket*)m_UpstreamMemory->allocate(sizeof(bucket) * ME_BUCKETCOUNT, "POOLALLOCATOR: Allocating Extra Pool");
			Count += ME_BUCKETCOUNT;

			// Pool Initialization
			for (size_t i = 1; i < ME_BUCKETCOUNT; i++)
				new (expand + i - 1) bucket* (expand + i);

			// Connecting Pool
			(expand + ME_BUCKETCOUNT - 1)->next = m_nextFree;
			m_nextFree = expand;

			bucket** temp  = (bucket**)m_UpstreamMemory->allocate(sizeof(bucket**) * (m_PoolCount + 1),"POOLALLOCATOR: Expanding Upstream Leadger");
			new (temp) bucket* (*m_Pools); // Copying old expanded pool pointers
			new (temp + m_PoolCount) bucket* (expand); // Copying the new pool pointer
			m_UpstreamMemory->deallocate(m_Pools, sizeof(bucket*) * m_PoolCount, "POOLALLOCATOR: Deallocating old upstream leadger");
			m_Pools = temp;

			m_PoolCount++;

			// using recursion to allocate with newly allocated pool.
			return allocate(size);
		}
		virtual void* verified_allocate(void* end_ptr, const size_t& size) override
		{
			bucket* cur = reinterpret_cast<bucket*>(end_ptr);
			double ME_BUCKETCOUNT = (double)size / sizeof(bucket);
			size_t count = 0;

			if (ME_BUCKETCOUNT > size / sizeof(bucket))
				count = static_cast<size_t>(ME_BUCKETCOUNT + 1);
			else
				count = static_cast<size_t>(ME_BUCKETCOUNT);

			for (size_t i = 0; i <= count; i++)
			{
				if (cur->next - cur != 1)
					break;


				if (i == count)
				{
					m_nextFree = cur;
					return end_ptr;
				}
				cur = cur->next;
			}
			return nullptr;
		}
		virtual void deallocate(void* ptr, const size_t& size) noexcept override
		{
			double ME_BUCKETCOUNT = (double)size / sizeof(bucket);
			size_t count = 0;

			if (ME_BUCKETCOUNT > size / sizeof(bucket))
				count = static_cast<size_t>(ME_BUCKETCOUNT + 1);
			else
				count = static_cast<size_t>(ME_BUCKETCOUNT);

			bucket* cur = reinterpret_cast<bucket*>(ptr);
			for (size_t i = 1; i <= count; i++)
			{
				if (i == count)
				{
					cur->next = m_nextFree;
					m_nextFree = reinterpret_cast<bucket*>(ptr);
				}
				else
					cur->next = cur + 1;

				cur = cur->next;
			}
		}
		virtual void release() override
		{

			// Add a way to deallocate the other pools

			m_nextFree = m_PoolHead;

			for (size_t i = 1; i < ME_BUCKETCOUNT; i++)
				(m_PoolHead + (i - 1))->next = (m_PoolHead + i);
		}

		inline size_t getFreeMemory() const noexcept { return 0; }
	private:

		bucket* m_PoolHead, * m_nextFree, ** m_Pools; // a ledger of all pools
		size_t Size, Count, m_PoolCount;
	};
}
#endif // !POOLALLOCATOR
