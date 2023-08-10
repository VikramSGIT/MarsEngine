#pragma once

#include "MarsFlags.h"

#include "Core/Entity.h"
#include "Vender/MTL/Vector.h"
#include "Vender/MTL/Ref.h"
#include "RenderAPI/Texture.h"

#include "Vender/glm/glm/glm.hpp"
#include <Vender/glm/glm/gtc/quaternion.hpp>

/*
* TODO: Plan virtually exposed functions
*/
namespace ME
{
	/*
	-MeshDataTypeFlags contains information of data which are changed draw during a draw cycle. This allows to send lower bandwidth data to the gpu by reducing the MeshDataSent in every draw call. Needed to be implemented later.
	*/
	enum class MeshDataTypeFlags {
		None = 0,
		Translation = 1,
		Scale = 1 << 1,
		Rotation = 1 << 2,
	};

	struct MeshData2D {
		glm::vec2 translation = { 0.0f, 0.0f }, scale = { 1.0f, 1.0f }, centroid = { 0.0f, 0.0f };
		ME_DATATYPE rotation = 0.0f;
		uint32_t vertexoffset;
	};

	struct VERTEX2D
	{
		glm::vec2 Position, UV;
		ME_DATATYPE index = 0.0f;
	};

	struct MeshPrimitive
	{
		vector<VERTEX2D> vertex;
		vector<uint32_t> index;
		uint32_t m_VertexOffset = 0, m_IndexOffset = 0, m_StartIndex = 0;
	};

	class Mesh2D : public Entity
	{
	public:

		Mesh2D(const string& name);

		void BufferData(const vector<VERTEX2D>& vertex, const vector<uint32_t>& index);
		void SetTexture(const Ref<Texture>& texture);

		void Translate(const glm::vec2& XY);
		void Rotate(const ME_DATATYPE& degree);
		void Scale(const glm::vec2& XY);

		inline Ref<MeshData2D> getMeshData() const { return m_MeshData; }
		inline Ref<MeshPrimitive> getPrimitive() const { return m_Primitive; }
		inline Ref<Texture> getTexture() const { return m_TextureData; }
		const glm::vec2 GetCentroid() const;

	private:
		Ref<MeshData2D> m_MeshData;
		Ref<MeshPrimitive> m_Primitive;
		Ref<Texture> m_TextureData; // Will need to change to materials
	};
//
// Commenly used meshes
//
	static Ref<Mesh2D> genRect(const string& name, const glm::vec2& lb, const uint32_t& index = 0) {
		vector<VERTEX2D> vertexbuffer =
		{
			{{0.0f, 0.0f}, {0.0f, 0.0f}, (float)index},
			{{lb.x, 0.0f}, {1.0f, 0.0f}, (float)index},
			{{lb.x, lb.y}, {1.0f, 1.0f}, (float)index},
			{{0.0f, lb.y}, {0.0f, 1.0f}, (float)index}
		};
		vector<uint32_t> indexbuffer =
		{
			0, 1, 2,
			2, 3, 0
		};
		Ref<Mesh2D> out = CreateRef<Mesh2D>(name);
		out->BufferData(vertexbuffer, indexbuffer);
		return out;
	}
	static Ref<Mesh2D> GenQuad2D(const string& name, const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const glm::vec2& v4, const unsigned int& index = 0){
		vector<VERTEX2D> vertexbuffer;
		{
			VERTEX2D{ {v1.x, v1.y}, {0.0f, 0.0f}, (float)index },
			VERTEX2D{ {v2.x, v2.y}, {1.0f, 0.0f}, (float)index },
			VERTEX2D{ {v3.x, v3.y}, {1.0f, 1.0f}, (float)index },
			VERTEX2D{ {v4.x, v4.y}, {0.0f, 1.0f}, (float)index };
		};
		vector<uint32_t> indexbuffer =
		{
			0, 1, 2,
			2, 3, 0
		};
		Ref<Mesh2D> out = CreateRef<Mesh2D>(name);
		out->BufferData(vertexbuffer, indexbuffer);
		return out;
	}
}