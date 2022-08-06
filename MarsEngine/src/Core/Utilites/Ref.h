#ifndef REF
#define REF

#include "Core/Memory/MemoryManager.h"
#include "Core/Entity.h"

namespace ME
{
	/**
	 Ref is Reference smart pointer built over the Allocator system for MarsEngine. This class allows the user to have
	 multiple reference of a pointer.
	 TODO: Make this thread safe
	**/
	template<typename T, typename upstreammemory = alloc_dealloc_UpstreamMemory> struct ControlBlock
	{
		size_t Count;
		char Obj[sizeof(T)];
	};
	template<typename T, typename upstreammemory = alloc_dealloc_UpstreamMemory> class Ref
	{
	public:

		typedef ControlBlock<T, upstreammemory> block;
		T* Ptr;

		Ref() :Ptr(nullptr){}

		Ref(nullptr_t) :Ptr(nullptr) {}

		Ref (const Ref& other)
		{
			Ptr = other.Ptr;
			if (Ptr != nullptr)
				getControlBlock()->Count++;
		}

		Ref(Ref&& other) noexcept
		{
			Ptr = other.Ptr;
			other.Ptr = nullptr;
		}

		template<typename U> Ref(const Ref<U, upstreammemory>& other)
		{
			Ptr = reinterpret_cast<T*>(other.Ptr);
			if (Ptr != nullptr)
				getControlBlock()->Count++;
		}

		template<typename U> Ref(Ref<U, upstreammemory>&& other) noexcept
		{
			Ptr = reinterpret_cast<T*>(other.Ptr);
			other.Ptr = nullptr;
		}

		~Ref()
		{
			if (Ptr != nullptr)
			{
				getControlBlock()->Count--;
				if (getControlBlock()->Count == 0)
				{
					Ptr->~T();
					upstreammemory::stref->deallocate(getControlBlock(), std::string("REF: Deallocating Control Block for ") + typeid(T).name());
				}
			}
		}

		Ref& operator=(const Ref& other)
		{
			Ptr = other.Ptr;
			if (Ptr != nullptr)
				getControlBlock()->Count++;

			return *this;
		}

		Ref& operator=(Ref&& other) noexcept
		{
			Ptr = other.Ptr;
			other.Ptr = nullptr;

			return *this;
		}

		template<typename U> Ref& operator=(const Ref<U, upstreammemory>& other)
		{
			Ptr = reinterpret_cast<T*>(other.Ptr);
			if (Ptr != nullptr)
				getControlBlock()->Count++;

			return *this;
		}

		template<typename U> Ref& operator=(Ref<U, upstreammemory>&& other)
		{
			Ptr = reinterpret_cast<T*>(other.Ptr);
			other.Ptr = nullptr;

			return *this;
		}

		void reset()
		{
			if (getControlBlock()->Count == 1)
			{
				Ptr->~T();
				upstreammemory::stref->deallocate(getControlBlock(), "REF: Deallocating Control Block ");
			}
			Ptr = nullptr;
		}

		inline T* ptr() { return Ptr; }
		inline bool IsValid() const { return Ptr != nullptr; }

		T& operator*() { return *Ptr; }
		T* operator->() { return Ptr; }
		T const& operator*() const { return *Ptr; }
		T const* operator->() const { return Ptr; }
		bool operator==(const Ref& other) const { return other.Ptr == Ptr; }
		bool operator!=(const Ref& other) const { return other.Ptr != Ptr; }
		template<typename U> bool operator== (const Ref<U, upstreammemory>& other) const { return other.Ptr == Ptr; }
		template<typename U> bool operator!= (const Ref<U, upstreammemory>& other) const { return other.Ptr != Ptr; }
	private:

		block* getControlBlock() { return ((block*)((char*)Ptr - sizeof(size_t))); }
		template<typename T, typename ...Args, typename upstreammemory> friend Ref<T, upstreammemory> CreateRef(Args&& ...args);
	};
	template<typename T, typename ...Args, typename upstreammemory = alloc_dealloc_UpstreamMemory> Ref<T, upstreammemory> CreateRef(Args&& ...args)
	{
		Ref<T, upstreammemory> ref;
		ControlBlock<T, upstreammemory>* ptr = (ControlBlock<T, upstreammemory>*)(upstreammemory::stref->allocate(sizeof(ControlBlock<T, upstreammemory>), std::string("REF: Allocating Control Block for ") + typeid(T).name()));
		ref.Ptr = (T*)ptr->Obj;
		ptr->Count = 1;
		new (ref.Ptr) T(args...);

		return ref;
	}
}
#endif