#pragma once

#include "MarsHeader.h"

#include "Buffers.h"
#include "Shader.h"
#include "Vender/glm/glm/gtc/matrix_transform.hpp"
#include "Vender/glm/glm/glm.hpp"
#include "Addons/Addon.h"

#include <memory>
#include <string>
#include <algorithm>
#include <set>

namespace ME
{
	enum class ALLOCAT
	{
		NONE = 0, ONMESHPUSH, CUSTOM
	};
	enum class ALLOCMODE
	{
		NONE = 0, ALLATONE, DISTRIBUTED
	};
	struct VERTEX
	{
		ME_DATATYPE vertices[3] = { 0.0f, 0.0f, 0.0f };
		ME_DATATYPE texturecoord[2] = { 0.0f, 0.0f };
		ME_DATATYPE index = 0.0f;
	};

	class Mesh
	{
	public:
		Mesh(const std::string& name)
		:m_Name(name) { ME_PROFILE_TRACE_CALL(); }
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

		inline const std::vector<VERTEX>& GetVertices() const { return std::vector<VERTEX>(m_data.vertices, m_data.vertices + m_data.vsize); }
		inline const std::vector<unsigned int>& GetIndices() const { return std::vector<unsigned int>(m_data.indices, m_data.indices + m_data.isize); };
		inline const std::pair<ME_DATATYPE const*, unsigned int> GetVertexData() const { return std::make_pair((ME_DATATYPE*)m_data.vertices, m_data.vsize); }
		inline const std::pair<unsigned int const*, unsigned int> GetIndexData() const { return std::make_pair(m_data.indices, m_data.isize); }
		inline const std::string GetName() const { return m_Name; }
		const glm::vec3 GetCentroid() const;

		inline bool IsReady() const { return m_Ready; }

		friend Ref<Mesh> operator* (Ref<Mesh>& mesh, const glm::mat4 &mat)
		{

			ME_PROFILE_TRACE_CALL();

			glm::mat4 matrix = mat;
			for (unsigned int i = 0; i < mesh->m_data.vsize; i++)
			{
				VERTEX& vertex = mesh->m_data.vertices[i];
				glm::vec4 out(vertex.vertices[0], vertex.vertices[1], vertex.vertices[2], 1.0f);
				out = matrix * out;
				vertex.vertices[0] = out.x;
				vertex.vertices[1] = out.y;
				vertex.vertices[2] = out.z;
			}
			mesh->m_Ready = false;
			return mesh;
		}

	private:

		struct MeshArray
		{
			unsigned int vsize = 0ull, isize = 0ull;
			unsigned int* indices = nullptr;
			VERTEX* vertices = nullptr;
		};
		MeshArray m_data;

		std::vector<VERTEX> m_Vertices;
		std::vector<unsigned int> m_Indices;
		std::vector<VERTEX> m_ResetVertices;
		std::string m_Name;

		bool m_Ready = false;
		friend class MeshQueue;
	};

	class MeshQueue
	{
	public:
		MeshQueue();
		virtual void PushMesh(const Ref<Mesh>& mesh) = 0;
		virtual void PushMeshes(const std::vector<Ref<Mesh>>& meshes) = 0;
		virtual void PushAddon(ME::Addon::MeshAddon& addon) = 0;

		inline std::vector<Ref<Mesh>> GetMeshes() const { return m_Meshes; }

		inline unsigned int GetTotalVertices() const { return total_vertices; }
		inline unsigned int GetTotalIndices() const { return total_indices; }
		std::vector<glm::vec<2, unsigned int>>  GetUpdate();

		std::vector<Ref<Mesh>>::iterator begin() { return m_Meshes.begin(); }
		std::vector<Ref<Mesh>>::iterator end() { return m_Meshes.end(); }
		std::vector<Ref<Mesh>>::reverse_iterator rbegin() { return m_Meshes.rbegin(); }
		std::vector<Ref<Mesh>>::reverse_iterator rend() { return m_Meshes.rend(); }

		std::vector<Ref<Mesh>>::const_iterator begin() const { return m_Meshes.begin(); }
		std::vector<Ref<Mesh>>::const_iterator end() const { return m_Meshes.end(); }
		std::vector<Ref<Mesh>>::const_reverse_iterator rbegin() const { return m_Meshes.rbegin(); }
		std::vector<Ref<Mesh>>::const_reverse_iterator rend() const { return m_Meshes.rend(); }

	private:
		Ref<Renderer::VertexBufferLayout> m_Layout = CreateRef<Renderer::VertexBufferLayout>();

		std::vector<Ref<Mesh>> m_Meshes;
		unsigned int total_vertices = 0, total_indices = 0;
	};

	class StaticQueue : public MeshQueue
	{

	};

	class DynamicQueue : public MeshQueue
	{

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