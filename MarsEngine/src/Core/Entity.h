#ifndef ME_ENTITY
#define ME_ENTITY

#pragma once

#define ME_ENTITYINIT() ME::InitEntityDB()
#define ME_ENTITYCLEAR() ME::DestroyEntityDB()

#define ME_CLASS_CONNECT(X)											\
public: \
virtual bool isValid(const size_t& id = typeid(X).hash_code()) override { return typeid(X).hash_code() == id; }	\
virtual const char* getName() override {return typeid(X).name();}

#include "Utilites/UUID.h"

#include <set>
#include <unordered_map>

namespace ME
{

	void InitEntityDB();
	void DestroyEntityDB();

	class EntityDB;

	class Entity
	{
	public:
		bool valid;
		Entity();
		virtual ~Entity();

		UUID const GetUUID() { return m_UUID; }

		bool operator==(const Entity& right) const { return m_UUID == right.m_UUID; }

		virtual bool isValid(const size_t&) = 0;
		virtual const char* getName() = 0;
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
	private:
		std::set<Entity*> m_Database;
		
		static EntityDB* s_EntityDB;

		friend class Entity;
		friend void InitEntityDB();
		friend void DestroyEntityDB();
	};
}

#endif