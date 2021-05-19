#ifndef SCOPE
#define SCOPE

#include "MemoryManager.h"
namespace ME
{
	template<typename T, typename upstreammemory = alloc_dealloc_UpstreamMemory>
	class Scope
	{
	public:
		Scope()
			:Ptr(nullptr), m_UpstreamMemory(new upstreammemory) {}
		
		~Scope()
		{
			destruct(Ptr);
			m_UpstreamMemory->deallocate(Ptr, sizeof(T), "SCOPE: Deallocating scope");
		}

		T* get() const noexcept
		{
			return Ptr;
		}

		T& operator*() { return *Ptr; }
		T* operator->() const noexcept { return Ptr; }

		Scope(const Scope&) = delete;
		T& operator=(const Scope&) = delete;
	private:
		T* Ptr;
		UpstreamMemory* m_UpstreamMemory;

		template<typename ...Args>
		void construct(T* ptr, Args ...args)
		{
			new (ptr) T(args...);
		}

		void destruct(T* ptr)
		{
			ptr->~T();
		}
		template<typename ...Args>
		Scope(Args ...args)
			:m_UpstreamMemory(new upstreammemory)
		{
			T* ptr = static_cast<T*>(m_UpstreamMemory->allocate(sizeof(T), "SCOPE: Initializing scope"));
			Ptr = ptr;
			construct(Ptr, args...);
		}
		template<typename T, typename ...Args, typename upstreammemory> friend Scope<T, upstreammemory> CreateScope(Args ...args);
	};
	template<typename T, typename ...Args, typename upstreammemory = alloc_dealloc_UpstreamMemory>
	Scope<T, upstreammemory> CreateScope(Args ...args) 
	{
		return Scope<T, upstreammemory>(args...);
	}
}
#endif // !SCOPE
