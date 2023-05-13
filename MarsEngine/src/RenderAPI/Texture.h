#ifndef ME_TEXTURE
#define ME_TEXURE

#pragma once

#include "MarsHeader.h"
#include "Core/Entity.h"
#include "Core/Utilites/Ref.h"
#include "Core/Utilites/String.h"

namespace ME
{
	struct TextureDimensions
	{
		int32_t Width = 0;
		int32_t Height = 0;
		int32_t BPP = 0;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual TextureDimensions GetDimensions() const = 0;
		virtual void SetData(void* data, const uint32_t& size) = 0;
		virtual void Bind(uint32_t slot = 0) = 0;
	};

	class Texture2D : public Texture
	{
		public:
		static Ref<Texture> Create(const string& path);
	};
}
#endif
