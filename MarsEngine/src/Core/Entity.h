#ifndef ME_ENTITY
#define ME_ENTITY

#pragma once

#define ME_ENTITYINIT() ME::InitEntityDB()
#define ME_ENTITYCLEAR() ME::DestroyEntityDB()

#define ME_CLASS_CONNECT(X) getName=[]()-> const char* {return X;}; InitEntity()

#include "Utilites/UUID.h"

#include <set>

namespace ME
{

	void InitEntityDB();
	void DestroyEntityDB();

	class EntityDB;

	class Entity
	{
	public:
		Entity();
		virtual ~Entity();

		UUID const GetUUID() { return m_UUID; }

		bool operator==(const Entity& right) const { return m_UUID == right.m_UUID; }

		void InitEntity();
		const char* (*getName)(void);
	private:
		UUID m_UUID;

		friend class EntityDB;
	};

	class EntityDB
	{
	public:
		static const EntityDB const* get() { return s_EntityDB; }

		inline Entity* getEntity(const UUID& uuid) 
		{
			for (auto e : m_Database)
			{
				if (e->m_UUID == uuid)
					return e;
			}
			return nullptr;
		}

		inline size_t getAlive() { return m_Database.size(); } 
		inline bool validate(const char* name) { return m_NameBase.end() != m_NameBase.find(name); }

	private:
		std::set<Entity*> m_Database;
		std::set<std::string> m_NameBase;
		friend class Entity;

		static EntityDB* s_EntityDB;

		friend class Entity;
		friend void InitEntityDB();
		friend void DestroyEntityDB();
	};
}

#endif