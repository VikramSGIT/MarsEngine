#include "Entity.h"

#include "Core/Logger.h"
#include "Memory/MemoryManager.h"

ME::EntityDB* ME::EntityDB::s_EntityDB = nullptr;

void ME::InitEntityDB()
{
	ME::EntityDB::s_EntityDB = new EntityDB();
}

void ME::DestroyEntityDB()
{ 
	ME_CORE_ERROR(ME::EntityDB::s_EntityDB->m_Database.size() != 0, std::to_string(ME::EntityDB::s_EntityDB->m_Database.size()).c_str());
	for (auto a : ME::EntityDB::s_EntityDB->m_Database)
		std::cout << a->getName() << std::endl; //replace it with logger macros

	delete ME::EntityDB::s_EntityDB;
}


ME::Entity::Entity()
{
	EntityDB::s_EntityDB->m_Database.insert(this);
}

ME::Entity::~Entity()
{
	EntityDB::s_EntityDB->m_Database.erase(EntityDB::s_EntityDB->m_Database.find(this));
}
