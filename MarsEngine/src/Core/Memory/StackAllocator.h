#ifndef STACKALLOCATOR
#define STACKALLOCATOR

#include "MemoryManager.h"

#define ME_STACKSIZE 128

namespace ME
{
	template<typename upstreammemory = alloc_dealloc_UpstreamMemory>
	class StackAllocator : public MemoryManager
	{
	public:
		StackAllocator()
			:MemoryManager(new upstreammemory), Capacity(ME_STACKSIZE), Size(0)
		{
			Head = m_UpstreamMemory->allocate(ME_STACKSIZE, "STACKALLOCATOR: Initializing stack")
		}
		~StackAllocator()
		{
			m_UpstreamMemory->deallocate(Head, ME_STACKSIZE, "STACKALLOCATOR: Deintializing stack");
		}

		void* allocate(const size_t& size) override
		{
			char* res = Tail;
			Tail += size;
			return res;
		}

		void deallocate(void* ptr, const size_t& size) noexcept override
		{
			if (Head - Tail >= 0)
				Tail = static_cast<char*>(ptr);
			else
				Tail = Head;
		}

		void release() override
		{
			Tail = Head
		}

		size_t getFreeMemory() const override { return Capacity; }
	private:
		void expand(const size_t& size)
		{
			char* ptr = m_UpstreamMemory->allocate(size + Size + Capacity, "STACKALLOCATOR: Expanding stack");
			memcpy(ptr, Head, Size + Capacity);
			m_UpstreamMemory->deallocate(Head, Size + Capacity, "STACKALLOCATOR: Destroying old memory");
			Head = ptr;
			Tail = Head + Size;
		}
		char* Head, Tail;
		size_t Size, Capacity;
	};
}
#endif // !STACKALLOCATOR
