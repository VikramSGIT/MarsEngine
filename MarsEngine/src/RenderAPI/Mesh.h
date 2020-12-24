#pragma once

#include "MarsHeader.h"

#include "Buffers.h"
#include "Shader.h"
#include "Vender/OGLM/oglm.h"
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

		inline std::vector<VERTEX>* GetVertices() { return &m_Vertices; }
		inline std::vector<unsigned int>* GetIndices() { return &m_Indices; }
		inline oglm::Matrix4<ME_DATATYPE> GetModelMat() { return m_Model; }

		inline bool IsReady() const { return Ready; }

		friend Ref<Mesh> operator * (Ref<Mesh> mesh, const oglm::Matrix4<ME_DATATYPE> &mat)
		{
			Scope<VERTEX[]> vertex = CreateScope<VERTEX[]>(mesh->m_Vertices.size());

			for (int i = 0; i < mesh->m_Vertices.size(); i++)
			{
				vertex[i] = mesh->m_Vertices.at(i);
				oglm::vec4<ME_DATATYPE> out(vertex[i].vertices[0], vertex[i].vertices[1], vertex[i].vertices[2], 1.0f);
				out = out * mat;
				vertex[i].vertices[0] = out.x;
				vertex[i].vertices[1] = out.y;
				vertex[i].vertices[2] = out.z;
			}

			mesh->UpdateVertices(vertex.get(), mesh->m_Vertices.size());
			return mesh;
		}

	private:
		std::vector<VERTEX> m_Vertices;
		std::vector<unsigned int> m_Indices;
		oglm::Matrix4<ME_DATATYPE> m_Model;
		std::string m_Name;
		bool Ready = false;
	};

	class MeshQueue
	{
	public:
		MeshQueue()
			:vertexbuffer(nullptr), indexbuffer(nullptr) {}

		void PushMesh(Ref<Mesh> mesh);
		void ClearBuffer() const;

		inline Ref<VertexBufferLayout> GetLayout() const { return m_Layout; }
		inline std::vector<Ref<Mesh>> GetMeshes() const { return m_Meshes; }

		inline unsigned int GetTotalVertices() const { return total_vertices; }
		inline unsigned int GetTotalIndices() const { return total_indices; }
		std::vector<oglm::vec2<unsigned int>> GetUpdate();

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