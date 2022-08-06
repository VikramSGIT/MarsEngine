#ifndef ME_VECTOR
#define ME_VECTOR

#include "Core/Memory/MemoryManager.h"

#include <mutex>

#define ME_DEFAULT_VECTOR_SIZE 3

namespace ME
{
	template<typename T> struct Iterator 
	{
		T* m_begin, m_end;
		T* begin() { return m_begin; }
		T* end() { return m_end; }
	};
	template<typename T, typename upstreammemory = alloc_dealloc_UpstreamMemory>
	class Vector 
	{
	public:
		Vector()
			:m_Capacity(ME_DEFAULT_VECTOR_SIZE), m_Size(0)
		{
			m_Head = (T*)upstreammemory::stref->allocate(sizeof(T) * m_Capacity, "VECTOR: Initialization");
			m_Tail = m_Head;
		}
		~Vector()
		{
			auto it = begin();
			while (it != end())
			{
				it->~T();
				it++;
			}
			upstreammemory::stref->deallocate(m_Head, "VECTOR: Deinitializing");
		}

		void push_back(const T& element)
		{
			if (m_Capacity == 0)
				expand(ME_DEFAULT_VECTOR_SIZE);

			new (m_Tail) T(element);
			m_Tail++;
			m_Size++;
			m_Capacity--;
		}
		void push(T* pos, const T& element)
		{
			ME_MEM_ERROR(belongs(pos), "VECTOR: Position out of scope");

			if (m_Capacity == 0)
				expand(ME_DEFAULT_VECTOR_SIZE);

			T* temp = alloc<T>(end() - pos);
			memcpy(temp, pos, (char*)end() - (char*)pos);

			new (pos) T(element);
			memcpy(pos + 1, temp, (char*)end() - (char*)pos);
			dealloc(temp);
			m_Tail++;
			m_Size++;
			m_Capacity--;
		}
		template<typename ...Args>
		void emplace_back(Args&& ...args)
		{
			if (m_Capacity == 0)
				expand(ME_DEFAULT_VECTOR_SIZE);

			new (m_Tail) T(args...);
			m_Tail++;
			m_Size++;
			m_Capacity--;
		}

		template<typename ...Args>
		void emplace(T* pos, Args&& ...args)
		{
			ME_MEM_ERROR(belongs(pos), "VECTOR: Position out of scope");

			if(m_Capacity == 0)
				expand(ME_DEFAULT_VECTOR_SIZE);

			T* temp = alloc<T>(end() - pos);
			memcpy(temp, pos, (char*)end() - (char*)pos);

			new (pos) T(args...);
			memcpy(pos + 1, temp, (char*)end() - (char*)pos);
			dealloc(temp);
			m_Tail++;
			m_Size++;
			m_Capacity--;
		}

		void reserve(const size_t& size)
		{
			if (m_Capacity < size)
				expand(size);
		}

		void erase(T* pos)
		{
			ME_MEM_ERROR(belongs(pos), "VECTOR: Position out of scope");

			pos++;
			T* temp = alloc<T>(end() - pos);
			memcpy(temp, pos, (char*)end() - (char*)pos);
			memcpy(pos - 1, temp, (char*)end() - (char*)pos);
			dealloc(temp);
			m_Tail--;
			m_Size--;
			m_Capacity++;
		}

		T& at(size_t index) 
		{
			if (index >= m_Size)
				ME_MEM_ERROR(true, "VERTOR: Index out of range!!")
			
				return *(m_Head + index);
		}
		T& operator[](size_t index)
		{ 
			if (index >= m_Size)
				ME_MEM_ERROR(true, "VECTOR: Index out of range!!")

				return *(m_Head + index);
		}

		void clear() 
		{ 
			auto it = m_Head;
			while (it != m_Tail)
			{
				it->~T();
				it++;
			}
			m_Tail = m_Head;
			m_Capacity += m_Size;
			m_Size = 0;

		}
		void release() 
		{
			auto it = begin();
			while (it != end())
			{
				it->~T();
				it++;
			}

			m_Tail = m_Head;
			upstreammemory::stref->deallocate(m_Head, "VECTOR: Deinitializing");
			m_Size = 0;
			m_Capacity = 0;
		}

		size_t size() const noexcept { return m_Size; }
		T* begin() noexcept { return m_Head; }
		T* end() noexcept { return m_Tail; }
		const T* begin() const noexcept { return m_Head; }
		const T* end() const noexcept { return m_Tail; }

	private:
		void expand(const size_t& count)
		{
			T* ptr = (T*)upstreammemory::stref->reallocate((void*&)m_Head, count * sizeof(T), "VECTOR: Expanding");
			m_Head = ptr;
			m_Tail = ptr + m_Size;
			m_Capacity += count;
		}

		bool belongs(T* ptr) { return (ptr - begin() >= 0) && (end() - ptr >= 0); }
		T *m_Head, *m_Tail;
		size_t m_Capacity, m_Size;
	};
}
#endif