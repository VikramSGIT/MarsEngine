#ifndef VECTOR
#define VECTOR

#include "MemoryManager.h"

#define ME_DEFAULT_VECTOR_SIZE 2

namespace ME
{
	template<typename T>
	class Vector
	{
	public:
		Vector(UpstreamMemory* upstreammemory = set_alloc_dealloc_UpstreamMemory())
			:m_Capacity(ME_DEFAULT_VECTOR_SIZE), m_Size(0), m_UpstreamMemory(upstreammemory)
		{
			m_Head = (T*)m_UpstreamMemory->allocate(sizeof(T) * m_Capacity, "VECTOR: Initialization");
			m_Tail = m_Head;
		}
		~Vector()
		{
			m_UpstreamMemory->deallocate(m_Head, sizeof(T) * (m_Size + m_Capacity), "VECTOR: Deinitializing");
		}

		void push_back(const T& element)
		{
			if (m_Capacity < 1)
			{
				expand(ME_DEFAULT_VECTOR_SIZE);
			}

			new (m_Tail) T(element);
			m_Tail++;
			m_Size++;
			m_Capacity--;
		}
		template<typename ...Args>
		void emplace_back(const Args&& ...args)
		{
			if (m_Capacity < 1)
			{
				expand(ME_DEFAULT_VECTOR_SIZE);
			}

			new (m_Tail) T(args...);
			m_Tail++;
			m_Size++;
			m_Capacity--;
		}

		void reserve(const size_t& size)
		{
			if (m_Capacity < size)
			{
				expand(size);
			}
		}

		T& at(size_t index) 
		{
			if (index >= m_Size)
				ME_MEMERROR(true, "Index out of range!!")
			
				return *(m_Head + index);
		}
		T& operator[](size_t index)
		{ 
			if (index >= m_Size)
				ME_MEMERROR(true, "Index out of range!!")

				return *(m_Head + index);
		}

		size_t size() const noexcept { return m_Size; }
		T* begin() const noexcept { return m_Head; }
		T* end() const noexcept { return m_Tail; }

	private:
		void expand(const size_t& size)
		{
			T* ptr = (T*)m_UpstreamMemory->reallocate(m_Head + m_Size + m_Capacity, size, "VECTOR: Verified allocate expantion");
			if (ptr == nullptr)
			{
				ptr = (T*)m_UpstreamMemory->allocate(m_Size + m_Capacity + size, "VECTOR: Allocating as verified_allocate failed");

				memcpy((void*)(ptr), (void*)(m_Head), sizeof(T) * m_Size);

				m_UpstreamMemory->deallocate(m_Head, sizeof(T) * (m_Size + m_Capacity), "VECTOR: Deallocating old memory");
				m_Head = ptr;
				m_Tail = ptr + m_Size;
			}
			m_Capacity += size;
		}
		UpstreamMemory* m_UpstreamMemory;
		T *m_Head, *m_Tail;
		size_t m_Capacity, m_Size;
	};
}
#endif // !VECTOR