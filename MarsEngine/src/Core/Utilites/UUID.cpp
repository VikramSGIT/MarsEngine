#include "UUID.h"

#include <random> 

static std::random_device rand_device;
static std::mt19937_64 random(rand_device());
static std::uniform_int_distribution<uint64_t> dist;


namespace ME
{
	UUID::UUID()
		:m_UUID(dist(random)) {}

	UUID::UUID(uint64_t uuid)
		:m_UUID(uuid) {}
}