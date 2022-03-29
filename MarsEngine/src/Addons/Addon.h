#ifndef ME_ADDON
#define ME_ADDON

#include "MarsHeader.h"

namespace ME
{
	class Mesh;
	namespace Addon
	{
		class MeshAddon
		{
		public:
			MeshAddon() = default;
			virtual ~MeshAddon() = default;

			virtual const std::vector<Ref<ME::Mesh>>& GetMeshes() const = 0;
		private:
		};
	}
}

#endif