#pragma once

#include "MarsHeader.h"

#include "Buffers.h"
#include "Shader.h"
#include "Vender/glm/glm/gtc/matrix_transform.hpp"
#include "Vender/glm/glm/glm.hpp"
#include <memory>
#include <algorithm>

namespace Renderer
{
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
			:m_Name(name) {}
		~Mesh() = default;

		void BufferVertices(const VERTEX* vertex, const unsigned int& count);
		void BufferIndices(const unsigned int* data, const unsigned int& count);

		void UpdateVertices(const VERTEX* vertex, const unsigned int& count);
		void UpdateIndices(const unsigned int* data, const unsigned int& count);

		void SetReady(bool ready) { Ready = ready; }
		void SetReset(const VERTEX* vertex, const unsigned int& count);
		void Reset();

		inline const std::vector<VERTEX> GetVertices() { return m_Vertices; }
		inline const std::vector<unsigned int> GetIndices() { return m_Indices; }
		inline glm::mat4 GetModelMat() { return m_Model; }
		inline std::string GetName() const { return m_Name; }

		inline bool IsReady() const { return Ready; }

		friend Ref<Mesh> operator* (Ref<Mesh>& mesh, const glm::mat4 &mat)
		{

			ME_PROFILE_TRACE_CALL();

			glm::mat4 matrix = mat;
			std::vector<VERTEX> vertices = mesh->m_Vertices;
			mesh->m_Vertices.clear();
			for (int i = 0; i < vertices.size(); i++)
			{
				VERTEX vertex;
				vertex = vertices.at(i);
				glm::vec4 out(vertex.vertices[0], vertex.vertices[1], vertex.vertices[2], 1.0f);
				out = matrix * out;
				vertex.vertices[0] = out.x;
				vertex.vertices[1] = out.y;
				vertex.vertices[2] = out.z;
				mesh->m_Vertices.emplace_back(vertex);
			}
			mesh->Ready = false;
			return mesh;
		}

	private:
		std::vector<VERTEX> m_Vertices;
		std::vector<unsigned int> m_Indices;
		std::vector<VERTEX> m_ResetVertices;
		glm::mat4 m_Model = glm::identity<glm::mat4>();
		std::string m_Name;
		bool Ready = false;
	};

	class MeshQueue
	{
	public:
		MeshQueue()
			:vertexbuffer(nullptr), indexbuffer(nullptr) {}

		void PushMesh(const Ref<Mesh>& mesh);
		void PushMeshes(const std::vector<Ref<Mesh>>& meshes);
		void ClearBuffer() const;

		inline Ref<VertexBufferLayout> GetLayout() const { return m_Layout; }
		inline std::vector<Ref<Mesh>> GetMeshes() const { return m_Meshes; }

		inline unsigned int GetTotalVertices() const { return total_vertices; }
		inline unsigned int GetTotalIndices() const { return total_indices; }
		std::vector<glm::uvec2> GetUpdate();

		std::vector<Ref<Mesh>>::iterator begin() { return m_Meshes.begin(); }
		std::vector< Ref<Mesh>>::iterator end() { return m_Meshes.end(); }
		std::vector< Ref<Mesh>>::reverse_iterator rbegin() { return m_Meshes.rbegin(); }
		std::vector< Ref<Mesh>>::reverse_iterator rend() { return m_Meshes.rend(); }

		std::vector<Ref<Mesh>>::const_iterator begin() const { return m_Meshes.begin(); }
		std::vector<Ref<Mesh>>::const_iterator end() const { return m_Meshes.end(); }
		std::vector<Ref<Mesh>>::const_reverse_iterator rbegin() const { return m_Meshes.rbegin(); }
		std::vector< Ref<Mesh>>::const_reverse_iterator rend() const { return m_Meshes.rend(); }

		inline const ME_DATATYPE* GetVertexBuffer() const { return vertexbuffer; }
		inline const unsigned int* GetIndexBuffer() const { return indexbuffer; }

	private:
		std::vector<Ref<Mesh>> m_Meshes;
		ME_DATATYPE* vertexbuffer;
		unsigned int* indexbuffer;
		Ref<VertexBufferLayout> m_Layout = CreateRef<VertexBufferLayout>();
		unsigned int total_vertices = 0, total_indices = 0;
	};
}