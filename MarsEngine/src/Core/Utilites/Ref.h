#ifndef REF
#define REF

#include "Core/Memory/MemoryManager.h"
#include "Core/Entity.h"

namespace ME
{
	/**
	 Ref is Reference smart pointer built over the Allocator system of MarsEngine. This class allows the user to have
	 multiple reference of a pointer.
	 TODO: Need to make a better implementation for UpstreamMemory initializer.
	**/
	template<typename T, typename upstreammemory = alloc_dealloc_UpstreamMemory> class ControlBlock
	{
	public:
		ControlBlock()
			:Count(0), Size(0) {}

		void inc() { Count++; }
		void dec() { Count--; }

		int Count, Size;
		char Obj[sizeof(T)];
	};
	template<typename T, typename upstreammemory = alloc_dealloc_UpstreamMemory> class Ref
	{
	public:
		ControlBlock<T, upstreammemory>* m_ControlBlock;

		Ref() :m_ControlBlock(nullptr){}

		Ref(nullptr_t) :m_ControlBlock(nullptr) {}

		Ref (const Ref& other)
		{
			m_ControlBlock = other.m_ControlBlock;
			if(m_ControlBlock != nullptr)
				m_ControlBlock->inc();
		}

		Ref(Ref&& other) noexcept
		{
			m_ControlBlock = other.m_ControlBlock;
			other.m_ControlBlock = nullptr;
		}

		template<typename U> Ref(const Ref<U, upstreammemory>& other)
		{
			m_ControlBlock = reinterpret_cast<ControlBlock<T, upstreammemory>*>(other.m_ControlBlock);
			if (m_ControlBlock != nullptr)
				m_ControlBlock->inc();
		}

		template<typename U> Ref(Ref<U, upstreammemory>&& other) noexcept
		{
			m_ControlBlock = reinterpret_cast<ControlBlock<T, upstreammemory>*>(other.m_ControlBlock);
			other.m_ControlBlock = nullptr;
		}

		~Ref()
		{
			if (m_ControlBlock != nullptr)
			{
				m_ControlBlock->dec();
				if (m_ControlBlock->Count == 0)
				{
					((T*)m_ControlBlock->Obj)->~T();
					upstreammemory::stref->deallocate(m_ControlBlock, m_ControlBlock->Size, std::string("REF: Deallocating Control Block for ") + typeid(T).name());
				}
			}
		}

		Ref& operator=(const Ref& other)
		{
			m_ControlBlock = other.m_ControlBlock;
			if (m_ControlBlock != nullptr)
				m_ControlBlock->inc();

			return *this;
		}

		Ref& operator=(Ref&& other) noexcept
		{
			m_ControlBlock = other.m_ControlBlock;
			other.m_ControlBlock = nullptr;

			return *this;
		}

		template<typename U> Ref& operator=(const Ref<U, upstreammemory>& other)
		{
			m_ControlBlock = reinterpret_cast<ControlBlock<T, upstreammemory>*>(other.m_ControlBlock);
			if (m_ControlBlock != nullptr)
				m_ControlBlock->inc();

			return *this;
		}

		template<typename U> Ref& operator=(Ref<U, upstreammemory>&& other)
		{
			m_ControlBlock = reinterpret_cast<ControlBlock<T, upstreammemory>*>(other.m_ControlBlock);
			other.m_ControlBlock = nullptr;

			return *this;
		}

		void reset()
		{
			if (m_ControlBlock->Count == 1)
			{
				((T*)m_ControlBlock->Obj)->~T();
				upstreammemory::stref->deallocate(m_ControlBlock, sizeof(ControlBlock<T, upstreammemory>), "REF: Deallocating Control Block");
			}
			m_ControlBlock = nullptr;
		}

		inline T* ptr() { return (T*)m_ControlBlock->Obj; }

		T& operator*() { return *reinterpret_cast<T*>(m_ControlBlock->Obj); }
		T* operator->() { return reinterpret_cast<T*>(m_ControlBlock->Obj); }
		T const& operator*() const { return *reinterpret_cast<T*>(m_ControlBlock->Obj); }
		T const* operator->() const { return reinterpret_cast<T*>(m_ControlBlock->Obj); }
		bool operator==(const Ref& other) { return other.m_ControlBlock == m_ControlBlock; }
		bool operator!=(const Ref& other) { return other.m_ControlBlock != m_ControlBlock; }
		template<typename U> bool operator==(const Ref<U, upstreammemory>& other) { return other.m_ControlBlock == m_ControlBlock; }
		template<typename U> bool operator!=(const Ref<U, upstreammemory>& other) { return other.m_ControlBlock != m_ControlBlock; }
	private:
		friend Ref;
		template<typename T, typename ...Args, typename upstreammemory> friend auto CreateRef(Args&& ...args);
	};
	template<typename T, typename ...Args, typename upstreammemory = alloc_dealloc_UpstreamMemory> auto CreateRef(Args&& ...args) 
	{
		Ref<T, upstreammemory> ref;
		ref.m_ControlBlock = (ControlBlock<T, upstreammemory>*)(upstreammemory::stref->allocate(sizeof(ControlBlock<T, upstreammemory>), std::string("REF: Allocating Control Block for ") + typeid(T).name()));
		ref.m_ControlBlock->Count = 1;
		ref.m_ControlBlock->Size = sizeof(ControlBlock<T, upstreammemory>);
		new (ref.m_ControlBlock->Obj) T(args...);

		return ref;
	}
}
#endif