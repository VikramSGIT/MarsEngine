#ifndef REF
#define REF

#include "MemoryManager.h"

namespace ME
{
	template<typename T, typename upstreammemory = alloc_dealloc_UpstreamMemory> class Ref
	{
	public:
		Ref()
			:ptr_count(nullptr), Ptr(nullptr), m_UpstreamMemory(new upstreammemory) {}

		Ref(const Ref& Reference) noexcept
			:Ptr(Reference.Ptr), ptr_count(Reference.ptr_count), m_UpstreamMemory(Reference.m_UpstreamMemory)
		{
			if (ptr_count != nullptr) 
			{
				*ptr_count += 1;
			}
		}

		Ref(const Ref&& Reference) noexcept
			:Ptr(Reference.Ptr), ptr_count(Reference.ptr_count), m_UpstreamMemory(Reference.m_UpstreamMemory)
		{
			if (ptr_count != nullptr) 
			{
				*ptr_count += 1;
			}
		}

		~Ref()
		{
			if (ptr_count != nullptr)
			{
				if (*ptr_count == 1) 
				{
					destruct(Ptr);
					m_UpstreamMemory->deallocate(Ptr, sizeof(T), "REF: Deinitializing ref");
					m_UpstreamMemory->deallocate(ptr_count, sizeof(size_t), "REF: Deinitializing ref count");
				}
				else 
				{
					*ptr_count -= 1;
				}
			}
		}

		void reset() { dealloc(Ptr); }
		T get() const noexcept { return *Ptr; }

		T& operator*() noexcept { return *Ptr; }
		T* operator->() const noexcept { return Ptr; }

	private:

		template<typename ...Args>
		void construct(T* ptr, Args ...args)
		{
			new (ptr) T(args...);
		}

		void destruct(T* ptr)
		{
			ptr->~T();
		}

		size_t* ptr_count;
		T* Ptr;
		UpstreamMemory* m_UpstreamMemory;
		template<typename ...Args>
		Ref(Args ...args)
			:m_UpstreamMemory(new upstreammemory) 
		{
			Ptr = static_cast<T*>(m_UpstreamMemory->allocate(sizeof(T), "REF: Initializing ref"));
			construct(Ptr, args...);
			ptr_count = static_cast<T*>(m_UpstreamMemory->allocate(sizeof(size_t), "REF: Initializing ref count"));
			*ptr_count = 1;
		}
		template<typename T, typename ...Args, typename upstreammemory> friend Ref<T> CreateRef(Args&& ...args);
	};
	template<typename T, typename ...Args, typename upstreammemory = alloc_dealloc_UpstreamMemory> Ref<T> CreateRef(Args&& ...args) 
	{ 
		return Ref<T, upstreammemory>(std::forward<Args>(args)...); 
	}
}
#endif // !REF