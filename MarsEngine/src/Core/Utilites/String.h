#ifndef ME_STRING
#define ME_STRING

#pragma once

#include "Core/Memory/MemoryManager.h"
#include "Core/Entity.h"

namespace ME
{
	template <typename upstreammemory = ME::alloc_dealloc_UpstreamMemory> class String
	{
	public:	

		String()
			:m_Size(0), m_String((char*)upstreammemory::stref->allocate(1, "STRING: Initiating String")) 
		{
			m_String = '\0';
		}

		String(const char* string)
			:m_Size(strlen(string))
		{
			m_String = (char*)upstreammemory::stref->allocate(m_Size + 1, "STRING: Allocating String");
			memcpy(m_String, string, m_Size + 1);
		}
		String(const String& string)
		{
			m_Size = string.m_Size;
			m_String = (char*)upstreammemory::stref->allocate(m_Size + 1, "STRING: Allocating String");
			memcpy(m_String, string.m_String, m_Size + 1);
		}

		~String()
		{
			upstreammemory::stref->deallocate(m_String, "STRING: Deallocating String");
		}

		inline const char* c_str() const { return m_String; }

		String& operator+(const char& right)
		{
			String out(1 + m_Size);

			memcpy(out.m_String, m_String, m_Size);
			*(out.m_String + m_Size) = right;
			(out.m_String + m_Size + 1) = '/0';
			return out;
		}

		String& operator+(const char* right)
		{
			size_t r_size = strlen(right);
			String out(end + m_Size);

			memcpy(out.m_String, m_String, m_Size);
			memcpy(out.m_String + m_Size, right, r_size + 1);
			return out;
		}

		String& operator+(const String& right)
		{
			String out(end + right.m_Size);

			memcpy(out.m_String, m_String, m_Size);
			memcpy(out.m_String + m_Size, right.m_String, right.m_Size + 1);

			m_Size += right.m_Size;
			m_String = nstr;
			return out;
		}

		String& operator=(const String& right)
		{
			if (m_String != nullptr)
				upstreammemory::stref->deallocate(m_String, "STRING: Deallocating old String");
			
			m_Size = right.m_Size;
			m_String = (char*)upstreammemory::stref->allocate(m_Size + 1, "STRING: Allocating String");
			memcpy(m_String, right.m_String, m_Size + 1);
			return *this;
		}

		bool operator==(const String& right)
		{
			size_t l = 0, r = 0;
			while (l < m_Size || r < right.m_Size)
			{
				if (m_String[l] != right.m_String[r])
					return false;
				l++, r++;
			}
			if (l != m_Size && r != right.m_Size)
				return false;
			return true;
		}

		bool operator!=(const String& right)
		{
			size_t l = 0, r = 0;
			while (l < m_Size || r < right.m_Size)
			{
				if (m_String[l] != right.m_String[r])
					return true;
				l++, r++;
			}
			if (l != m_Size && r != right.m_Size)
				return true;
			return false;
		}

		String& operator+=(const char& right)
		{
			upstreammemory::stref->reallocate((void*&)m_String, 1, "STRING: Expanding String");

			*(m_String + m_Size) = right;
			*(m_String + m_Size + 1) = '/0';
			m_Size += 1;
			return *this;
		}

		String& operator+=(const char* right)
		{
			size_t r_size = strlen(right);
			upstreammemory::stref->reallocate((void*&)m_String, r_size, "STRING: Expanding String");

			memcpy(m_String + m_Size, right, r_size + 1);
			m_Size += r_size;
			return *this;
		}

		String& operator+=(const String& right)
		{
			upstreammemory::stref->reallocate((void*&)m_String, right.m_Size, "STRING: Expanding String");

			memcpy(m_String + m_Size, right.m_String, right.m_Size + 1);
			m_Size += right.m_Size;
			return *this;
		}

		char* begin() { return m_String; }
		char* end() { return m_String + m_Size; }
		const char* begin() const { return m_String; }
		const char* end() const { return m_String + m_Size; }
	private:
		char* m_String;
		size_t m_Size = 0;

		String (const size_t& size)
			:m_Size(size), m_String((char*)upstreammemory::stref->allocate(size + 1, "STRING: Initiating String")) {}
	};
	using string = String<alloc_dealloc_UpstreamMemory>;
}

#endif