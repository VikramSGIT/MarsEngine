#ifndef ME_MESH
#define ME_MESH

#pragma once

#include "Core/Entity.h"
#include "RenderAPI/Buffers.h"
#include "Addons/Addon.h"
#include "Core/Utilites/Ref.h"
#include "Core/Utilites/Vector.h"
#include "Core/Utilites/String.h"
#include "RenderAPI/Texture.h"

#include "Vender/glm/glm/glm.hpp"
#include "Vender/glm/glm/gtc/matrix_transform.hpp"

#include <functional>

/*
* TODO: Plan virtually exposed functions
*/
namespace ME
{
	struct VERTEX
	{
		ME_DATATYPE x = 0.0f, y = 0.0f, z = 0.0f, uv_x = 0.0f, uv_y = 0.0f, index = 0.0f;
	};

	struct MeshData
	{
		MeshData()
			:vertex(Vertex()), index(Index()) {}
		class Vertex
		{
		public:
			Vertex()
				:vertex(nullptr), reset_vertex(nullptr), m_Size(0), m_Offset(0) {}

			VERTEX* begin() { return vertex; }
			VERTEX* end() { return (vertex + m_Size); }

			const VERTEX* begin() const { return vertex; }
			const VERTEX* end() const { return (vertex + m_Size); }

			VERTEX* operator++() { return vertex++; }

			inline const unsigned int Size() const { return m_Size; }
			inline VERTEX* GetReset() { return reset_vertex; }
			inline const unsigned int GetOffset() const { return m_Offset; }

			void SetOffset(const unsigned int& offset) { m_Offset = offset; }

		private:
			VERTEX *vertex, *reset_vertex;
			unsigned int m_Size, m_Offset;

			friend class Mesh;
		};

		class Index
		{
		public:
			Index()
				:index(nullptr), m_Size(0), m_Offset(0), m_IndexOffset(0) {}

			unsigned int* begin() { return index; }
			unsigned int* end() { return (index + m_Size); }

			const unsigned* begin() const { return index; }
			const unsigned* end() const { return (index + m_Size);; }

			inline const unsigned int Size() const { return m_Size; }
			inline const unsigned int GetOffset() const { return m_Offset; }
			inline const unsigned int GetIndexOffset() const { return m_IndexOffset; }

			void SetOffset(const unsigned int& offset) { m_Offset = offset; }
			void SetIndexOffset(const unsigned int& indexoffset) { m_IndexOffset = indexoffset; }

		private:
			unsigned int* index;
			unsigned int m_Size, m_Offset, m_IndexOffset;

			friend class Mesh;
			friend class OpenGlRenderer;
		};

		Vertex vertex;
		Index index;
	};

	class Mesh : public Entity
	{	ME_CLASS_CONNECT(Mesh)
	public:

		Mesh(const string& name);
		Mesh(const Mesh& mesh);
		Mesh(Mesh&& mesh) noexcept;
		virtual ~Mesh();

		void BufferVertices(const VERTEX* vertex, const unsigned int& count);
		void BufferIndices(const unsigned int* data, const unsigned int& count);
		void SetTexture(const Ref<Texture>& texture);

		void Translate(const glm::vec3&);
		void TranslateTo(const glm::vec3& xyz);
		void Rotate(const glm::vec3& xyz);
		void Scale(const glm::vec3& xyz);
		void Transform(const glm::mat4& mat);

		void SetReset(const VERTEX* vertex);
		void SetReset(const std::vector<VERTEX>& vertices);
		void Reset();

		inline MeshData& GetMeshData() { return m_MeshData; }
		inline const MeshData& GetMeshData() const { return m_MeshData; }
		inline string GetName() const { return m_Name; }
		inline Ref<Texture> GetTexture() { return m_TextureData; }
		glm::vec3 getCentroid() const;

		Mesh operator* (const glm::mat4& mat);
	private:
		MeshData m_MeshData;
		string m_Name;
		Ref<Texture> m_TextureData;

		friend class MeshQueue;
	};
//
// Commenly used meshes
//
	static Ref<Mesh> GenQuad(const string& name, const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const glm::vec2& v4, const unsigned int& index = 0)
	{

		ME_PROFILE_TRACE_CALL();

		VERTEX vertexbuffer[] =
		{
			{v1.x, v1.y, 1.0f, 0.0f, 0.0f, (float)index},
			{v2.x, v2.y, 1.0f, 1.0f, 0.0f, (float)index},
			{v3.x, v3.y, 1.0f, 1.0f, 1.0f, (float)index},
			{v4.x, v4.y, 1.0f, 0.0f, 1.0f, (float)index}
		};
		unsigned int indexbuffer[] =
		{
			0, 1, 2,
			2, 3, 0
		};
		Ref<Mesh> out = CreateRef<Mesh>(name);
		out->BufferVertices(vertexbuffer, 4);
		out->BufferIndices(indexbuffer, 6);
		return out;
	}
//
// Generate Rectangle using diagonal points
	static Ref<Mesh> GenRect(const string& name, const glm::vec2& point1, const glm::vec2& point2, const unsigned int& index = 0)
	{

		ME_PROFILE_TRACE_CALL();

		VERTEX vertexbuffer[] =
		{
			{point1.x, point1.y, 1.0f, 0.0f, 0.0f, (float)index},
			{point2.x, point1.y, 1.0f, 1.0f, 0.0f, (float)index},
			{point2.x, point2.y, 1.0f, 1.0f, 1.0f, (float)index},
			{point1.x, point2.y, 1.0f, 0.0f, 1.0f, (float)index}
		};
		unsigned int indexbuffer[] =
		{
			0, 1, 2,
			2, 3, 0
		};
		Ref<Mesh> out = CreateRef<Mesh>(name);
		out->BufferVertices(vertexbuffer, 4);
		out->BufferIndices(indexbuffer, 6);
		return out;
	}
//
// Generates a quad using lenght and breath
	static Ref<Mesh> GenRect(const string& name, const glm::vec2& lb, const unsigned int& index = 0)
	{

		ME_PROFILE_TRACE_CALL();

		VERTEX vertexbuffer[] =
		{
			{0.0f, 0.0f, 1.0f, 0.0f, 0.0f, (float)index},
			{lb.x, 0.0f, 1.0f, 1.0f, 0.0f, (float)index},
			{lb.x, lb.y, 1.0f, 1.0f, 1.0f, (float)index},
			{0.0f, lb.y, 1.0f, 0.0f, 1.0f, (float)index}
		};
		unsigned int indexbuffer[] =
		{
			0, 1, 2,
			2, 3, 0
		};
		Ref<Mesh> out = CreateRef<Mesh>(name);
		out->BufferVertices(vertexbuffer, 4);
		out->BufferIndices(indexbuffer, 6);
		return out;
	}
//
// Quickest quad generation with 1x1 size
	static Ref<Mesh> QuickQuad(const unsigned int& index = 0) { return GenRect("Quad", {1.0f, 1.0f}, index); }
}

#endif