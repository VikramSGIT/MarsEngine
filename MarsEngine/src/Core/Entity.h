#pragma once

#include "MarsHeader.h"

#include "Utilites/UUID.h"
namespace ME
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(UUID uuid)
			:m_UUID(uuid) {}

		UUID const GetUUID() { return m_UUID; }

		bool operator==(const Entity& right) const { return m_UUID == right.m_UUID; }
	private:
		UUID m_UUID;
	};
}
