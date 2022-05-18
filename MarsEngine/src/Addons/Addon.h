#ifndef ME_ADDON
#define ME_ADDON

#pragma once

#include "Core/Utilites/Ref.h"

namespace ME
{
	class Mesh;
	class Mesh2D;
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

		class MeshAddon2D
		{
		public:
			MeshAddon2D() = default;
			virtual ~MeshAddon2D() = default;

			virtual const std::vector<Ref<ME::Mesh2D>>& GetMeshes() const = 0;
		private:
		};
	}
}

#endif