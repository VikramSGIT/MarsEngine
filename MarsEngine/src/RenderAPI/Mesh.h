#pragma once

#include "MarsHeader.h"

#include "Core/Entity.h"
#include "Buffers.h"
#include "Shader.h"
#include "Vender/glm/glm/gtc/matrix_transform.hpp"
#include "Vender/glm/glm/glm.hpp"
#include "Addons/Addon.h"

#include <memory>
#include <string>
#include <algorithm>
#include <set>
/*
* TODO: Need to implement mesh update call back
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
				:vertex(nullptr), reset_vertex(nullptr), m_Size(0) {}

			VERTEX* begin() { return vertex; }
			VERTEX* end() { return (vertex + m_Size); }

			const VERTEX* begin() const { return vertex; }
			const VERTEX* end() const { return (vertex + m_Size); }

			VERTEX* operator++() { return vertex++; }

			inline unsigned int Size() { return m_Size; }
			inline VERTEX* GetReset() { return reset_vertex; }
		private:
			VERTEX* vertex, * reset_vertex;
			size_t m_Size;

			friend class Mesh;
			friend class StaticQueue;
			friend class DynamicQueue;
		};

		class Index
		{
		public:
			Index()
				:index(nullptr), m_Size(0) {}

			unsigned int* begin() { return index; }
			unsigned int* end() { return (index + m_Size); }

			const unsigned* begin() const { return index; }
			const unsigned* end() const { return (index + m_Size);; }


			inline unsigned int Size() { return m_Size; }
		private:
			unsigned int* index;
			size_t m_Size;

			friend class Mesh;
			friend class StaticQueue;
			friend class DynamicQueue;
		};

		Vertex vertex;
		Index index;
	};

	class Mesh : public Entity
	{
	public:

		Mesh(const std::string& name)
			:m_Name(name), m_MeshData(MeshData()), m_Ready(false), m_Static(false) {}
		Mesh(const Mesh& mesh);
		Mesh(Mesh&& mesh) noexcept;
		~Mesh();

		void BufferVertices(const VERTEX* vertex, const unsigned int& count);
		void BufferIndices(const unsigned int* data, const unsigned int& count);

		void SetReady(bool ready) { m_Ready = ready; }
		void SetReset(const VERTEX* vertex, const unsigned int& count);
		void SetReset(const std::vector<VERTEX>& vertices);
		void Reset();

		void Transform(const glm::mat4& matrix);
		void Translate(const glm::vec3& XYZ);
		void TranslateTo(const glm::vec3& XYZ); //translate the mesh's centroid to the point specified
		void Rotate(const glm::vec3& XYZ);
		void Scale(const glm::vec3& XYZ);

		inline const MeshData GetMeshData() const { return m_MeshData; }
		inline const std::string GetName() const { return m_Name; }
		const glm::vec3 GetCentroid() const;

		inline bool IsReady() const { return m_Ready; }

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
			m_Ready = false;
			return *this;
		}

	private:
		MeshData m_MeshData;
		std::string m_Name;
		bool m_Ready, m_Static;

		friend class StaticQueue;
		friend class DynamicQueue;
	};

	class MeshQueue
	{
	public:
		virtual void PushMesh(const Ref<Mesh>& mesh) = 0;
		virtual void PushMeshes(const std::vector<Ref<Mesh>>& meshes) = 0;
		virtual void PushAddon(ME::Addon::MeshAddon& addon) = 0;

		virtual inline std::vector<Ref<Mesh>> GetMeshes() const = 0;

		virtual inline unsigned int GetTotalVertices() const = 0;
		virtual inline unsigned int GetTotalIndices() const = 0;
		inline Ref<Renderer::VertexBufferLayout> GetLayout() const { return m_Layout; }
		std::vector<glm::vec<2, unsigned int>>  GetUpdate();

		virtual Ref<Mesh> begin() = 0;
		virtual Ref<Mesh> end() = 0;

		virtual const Ref<Mesh> begin() const = 0;
		virtual const Ref<Mesh> end() const = 0;

	private:
		Ref<Renderer::VertexBufferLayout> m_Layout;
	};

	class StaticQueue : public MeshQueue
	{
	public:
		virtual void PushMesh(const Ref<Mesh>& mesh);
		virtual void PushMeshes(const std::vector<Ref<Mesh>>& mesh);
		virtual void PushAddon(ME::Addon::MeshAddon& addon);

		virtual inline std::vector<Ref<Mesh>> GetMeshes() const;

		virtual inline unsigned int GetTotalVertices() const;
		virtual inline unsigned int GetTotalIndices() const;
		std::vector<glm::vec<2, unsigned int>>  GetUpdate();

		virtual Ref<Mesh> begin();
		virtual Ref<Mesh> end();

		virtual const Ref<Mesh> begin() const;
		virtual const Ref<Mesh> end() const;

		void Allocate();

	private:
		std::vector<Ref<Mesh>> m_Meshes;

		VERTEX* VertexHead;
		unsigned int* IndexHead;
		unsigned int total_vertices, total_indices;
	};

	class DynamicQueue : public MeshQueue
	{
	public:
		virtual void PushMesh(const Ref<Mesh>& mesh);
		virtual void PushMeshes(const std::vector<Ref<Mesh>>& mesh);
		virtual void PushAddon(ME::Addon::MeshAddon& addon);

		virtual inline std::vector<Ref<Mesh>> GetMeshes() const;

		virtual inline unsigned int GetTotalVertices() const;
		virtual inline unsigned int GetTotalIndices() const;
		std::vector<glm::vec<2, unsigned int>>  GetUpdate();

		virtual Ref<Mesh> begin();
		virtual Ref<Mesh> end();

		virtual const Ref<Mesh> begin() const;
		virtual const Ref<Mesh> end() const;

	private:
		std::vector<Ref<Mesh>> m_Meshes;
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