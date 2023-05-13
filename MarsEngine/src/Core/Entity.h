#ifndef ME_ENTITY
#define ME_ENTITY

#pragma once

#include "Utilites/UUID.h"
#include "Utilites/Ref.h"
#include "Utilites/Vector.h"
#include "Utilites/String.h"

namespace ME
{
	class EntityType {
	public:
		EntityType() = delete;
		string getName() const { return m_Name; }

		bool operator==(EntityType const& right) { return m_Name == right.m_Name; }
		bool operator!=(EntityType const& right) { return m_Name != right.m_Name; }
	private:
		EntityType(const string& name)
			:m_Name(name) {}
		string m_Name;

		friend class Entity;
	};

	class Entity
	{
	public:
		Entity(const string& name, const string& type)
			:m_Name(name), m_Type(type) {};
		virtual ~Entity() {};

		bool operator==(const Entity& right) const { return m_UUID == right.m_UUID; }
		bool operator!=(const Entity& right) const { return m_UUID != right.m_UUID; }

		virtual void setName(const string& name) { m_Name = name; }

		virtual string getName() const { return m_Name; }
		virtual ref<Entity> getParent() { return m_Parent; }
		virtual vector<ref<Entity>>& getChildren() {return m_Children; }
		
		UUID GetUUID() const { return m_UUID; }
		EntityType const& getType() const { return m_Type; }
	private:
		UUID m_UUID;
		string m_Name;
		EntityType m_Type;
		ref<Entity> m_Parent;
		vector<ref<Entity>> m_Children;
	};
}

#endif