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
			upstreammemory::stref->deallocate(m_String, m_Size + 1, "STRING: Deallocating String");
		}

		inline const char* c_str() { return m_String; }

		String& operator+(const char* right)
		{
			bool expanded;
			size_t end = strlen(right);
			char* nstr = (char*)upstreammemory::stref->reallocate(m_String + m_Size + 1, end, "STRING: Expanding String");
			if (!(expanded = nstr))
				nstr = (char*)upstreammemory::stref->allocate(m_Size + end + 1, "STRING: Allocating new String as realloc failed");

			memcpy(nstr, m_String, m_Size);
			memcpy(nstr + m_Size, right, end + 1);

			if(!expanded)
				upstreammemory::stref->deallocate(m_String, m_Size + 1, "STRING: Deallocating old String");

			m_Size += end;
			m_String = nstr;
			return *this;
		}

		String& operator+(const String& right)
		{
			bool expanded;
			char* nstr = (char*)upstreammemory::stref->reallocate(right.m_Size, "STRING: Expanding String");
			if (!(expanded = nstr))
				nstr = (char*)upstreammemory::stref->allocate(m_Size + right.m_Size + 1, "STRING: Allocating new String as realloc failed");

			memcpy(nstr, m_String, m_Size);
			memcpy(nstr + m_Size, right.m_String, right.m_Size + 1);

			if (!expanded)
				upstreammemory::stref->deallocate(m_String, m_Size + 1, "STRING: Deallocating old String");

			m_Size += right.m_Size;
			m_String = nstr;
			return *this;
		}

		String& operator=(const String& right)
		{
			if (m_String != nullptr)
				upstreammemory::stref->deallocate(m_String, m_Size + 1, "STRING: Deallocating old String");
			
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

		char* begin() { return m_String; }
		char* end() { return m_String[m_Size]; }
		const char* const_begin() { return m_String; }
		const char* const_end() { return m_String[m_Size]; }
	private:
		char* m_String;
		size_t m_Size = 0;
	};
	using string = String<alloc_dealloc_UpstreamMemory>;
}

#endif
