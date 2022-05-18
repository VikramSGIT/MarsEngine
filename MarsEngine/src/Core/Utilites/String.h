#ifndef ME_STRING
#define ME_STRING

#pragma once

#include "Core/Memory/MemoryManager.h"
#include "Core/Entity.h"

namespace ME
{
	template <typename upstreammemory = alloc_dealloc_UpstreamMemory> class String : public Entity
	{
	public:	
		String()
			:m_Size(0), m_String((char*)upstreammemory::stref->allocate(1, "STRING: Allocating String"))
		{
			ME_CLASS_CONNECT("String");

			m_String = '\0';
		}

		String(const char* string)
			:m_Size(strlen(string))
		{
			ME_CLASS_CONNECT("String");
 
			m_String = (char*)upstreammemory::stref->allocate(m_Size + 1, "STRING: Allocating String");
			memcpy(m_String, string, m_Size + 1);
		}
		String(const String& string)
		{

			ME_CLASS_CONNECT("String");

			upstreammemory::stref->deallocate(m_String, m_Size + 1, "STRING: Deallocating old String");
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
	private:
		char* m_String;
		size_t m_Size = 0;
	};
}

#endif
