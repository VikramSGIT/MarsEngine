#ifndef ME_UUID
#define ME_UUID

#include "MarsHeader.h"

#include <xhash>

namespace ME
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);

		operator uint64_t() const { return m_UUID; }
		bool operator==(const UUID& right) const { return m_UUID == right.m_UUID; }
	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<ME::UUID>
	{
		std::size_t operator()(const ME::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}

	};
}

#endif