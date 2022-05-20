#ifndef ME_MESH
#define ME_MESH

#pragma once

#include "Core/Utilites/Vector.h"

#include "Core/Entity.h"
#include "RenderAPI\Buffers.h"
#include "Addons/Addon.h"
#include "Core/Utilites/Ref.h"

#include "Vender/glm/glm/glm.hpp"
#include "Vender/glm/glm/gtc/matrix_transform.hpp"
#include <string>
#include <functional>

/*
* TODO: Plan virtually exposed functions
*/
namespace ME
{
	struct VERTEX
	{
		ME_DATATYPE vertices[3] = { 0.0f, 0.0f, 0.0f };
		ME_DATATYPE texturecoord[2] = { 0.0f, 0.0f };
		ME_DATATYPE index = 0.0f;
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
			VERTEX* vertex, * reset_vertex;
			unsigned int m_Size, m_Offset;

			friend class Mesh;
		};

		class Index
		{
		public:
			Index()
				:index(nullptr), m_Size(0), m_Offset(0) {}

			unsigned int* begin() { return index; }
			unsigned int* end() { return (index + m_Size); }

			const unsigned* begin() const { return index; }
			const unsigned* end() const { return (index + m_Size);; }

			inline const unsigned int Size() const { return m_Size; }
			inline const unsigned int GetOffset() const { return m_Offset; }

			void SetOffset(const unsigned int& offset) { m_Offset = offset; }

		private:
			unsigned int* index;
			unsigned int m_Size, m_Offset;

			friend class Mesh;
			friend class OpenGlRenderer;
		};

		Vertex vertex;
		Index index;
	};

	class Mesh : public Entity
	{	ME_CLASS_CONNECT(Mesh)
	public:

		Mesh(const std::string& name)
			:m_Name(name), m_MeshData(MeshData()), Ready(true) {}
		Mesh(const Mesh& mesh);
		Mesh(Mesh&& mesh) noexcept;
		~Mesh();

		void BufferVertices(const VERTEX* vertex, const unsigned int& count);
		void BufferIndices(const unsigned int* data, const unsigned int& count);

		void SetReset(const VERTEX* vertex);
		void SetReset(const std::vector<VERTEX>& vertices);
		void SetReady(const bool& ready) { Ready = ready; }
		void Reset();

		void Transform(const glm::mat4& matrix);
		void Translate(const glm::vec3& XYZ);
		void TranslateTo(const glm::vec3& XYZ); //translate the mesh's centroid to the point specified
		void Rotate(const glm::vec3& XYZ);
		void Scale(const glm::vec3& XYZ);

		inline const MeshData& GetMeshData() const { return m_MeshData; }
		inline MeshData& GetMeshData() { return m_MeshData; }
		inline const std::string GetName() const { return m_Name; }
		inline const bool IsReady() { return Ready; }
		const glm::vec3 GetCentroid() const;

		Mesh operator* (const glm::mat4 &mat)
		{

			ME_PROFILE_TRACE_CALL();

			glm::mat4 matrix = mat;
			for (VERTEX& vertex : m_MeshData.vertex)
			{
				glm::vec4 out(vertex.vertices[0], vertex.vertices[1], vertex.vertices[2], 1.0f);
				out = matrix * out;
				vertex.vertices[0] = out.x;
				vertex.vertices[1] = out.y;
				vertex.vertices[2] = out.z;
			}
			return *this;

			callback(this, m_MeshData.vertex.m_Offset);
		}

	private:
		MeshData m_MeshData;

		std::function<void(Mesh*, const unsigned int&)> callback;
		std::string m_Name;
		bool Ready;

		friend class MeshQueue;
	};

	class MeshQueue // Render-Call-batcher
	{
	public:

		MeshQueue();

		virtual void PushMesh(const Ref<Mesh>& mesh);
		virtual void PushMeshes(const Vector<Ref<Mesh>>& meshes);
		virtual void PushMeshes(const std::vector<Ref<Mesh>>& meshes);
		virtual void PushAddon(ME::Addon::MeshAddon& addon);

		virtual inline std::vector<Ref<Mesh>> GetMeshes() const { return m_Meshes; }

		virtual inline size_t GetTotalVertices() const { return total_vertices; }
		virtual inline size_t GetTotalIndices() const { return total_indices; }
		virtual inline Ref<Renderer::VertexBufferLayout> GetLayout() const { return m_Layout; }
		virtual const std::vector<std::pair<Mesh*, unsigned int>> GetUpdate()
		{
			if (m_MeshUpdates.size())
			{
				std::vector<std::pair<Mesh*, unsigned int>> out = m_MeshUpdates;
				m_MeshUpdates.clear();
				return out;
			}
			m_MeshUpdates.clear();
			return m_MeshUpdates;
		}

		std::vector<Ref<Mesh>>::iterator begin() { return m_Meshes.begin(); }
		std::vector<Ref<Mesh>>::iterator end() { return m_Meshes.end(); }

		const std::vector<Ref<Mesh>>::const_iterator begin() const { return m_Meshes.begin(); }
		const std::vector<Ref<Mesh>>::const_iterator end() const { return m_Meshes.end(); }

	private:

		void MeshCallback(Mesh* mesh, const unsigned int& offset)
		{
			if (mesh->Ready)
			{
				m_MeshUpdates.emplace_back(mesh, offset);
				mesh->Ready = true;
			}
		}

		Ref<Renderer::VertexBufferLayout> m_Layout;
		std::vector <Ref<Mesh>> m_Meshes;
		std::vector<std::pair<Mesh*, unsigned int>> m_MeshUpdates;

		size_t total_vertices;
		unsigned int total_indices;
	};
//
// Commenly used meshes
//
	static Ref<Mesh> GenQuad(const std::string& name, const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const glm::vec2& v4, const unsigned int& index = 0)
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
	static Ref<Mesh> GenRect(const std::string& name, const glm::vec2& point1, const glm::vec2& point2, const unsigned int& index = 0)
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
	static Ref<Mesh> GenRect(const std::string& name, const glm::vec2& lb, const unsigned int& index = 0)
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