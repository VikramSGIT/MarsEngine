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
		float vertices[3];
		float texturecoord[2];
		float index;
	};

	class Mesh
	{
	public:
		Mesh(const std::string& name)
			:m_Name(name) {}
		~Mesh() = default;

		void BufferVertices(const VERTEX* vertex, const unsigned int& size);
		void BufferIndices(const unsigned int* data, const unsigned int& size);
		void BufferTextureCoord(const float* data, const unsigned int& size);
		void SetReady(bool ready) { Ready = ready; }

		inline std::vector<VERTEX>* GetVertices() { return &m_Vertices; }
		inline std::vector<unsigned int>* GetIndices() { return &m_Indices; }
		inline bool IsReady() const { return Ready; }

		friend Mesh operator * (Mesh &mesh, const oglm::Matrix4<float> &mat)
		{
			oglm::vec4<float> out;
			for (VERTEX &vertex : *mesh.GetVertices())
			{
				out = oglm::vec4<float>({ vertex.vertices[0], vertex.vertices[1], vertex.vertices[2] }, 1.0f) * mat;
				vertex.vertices[0] = out.x;
				vertex.vertices[1] = out.y;
				vertex.vertices[2] = out.z;
			}
			mesh.SetReady(false);
		}

	private:
		std::vector<VERTEX> m_Vertices;
		std::vector<unsigned int> m_Indices;
		std::string m_Name;
		bool Ready = false;
	};

	class MeshQueue
	{
	public:
		MeshQueue() = default;
		MeshQueue(const MeshQueue& mesh)
		{
			MeshQueue(meshqueue);
		}

		void PushMesh(Ref<Mesh> mesh);

		inline Ref<VertexBufferLayout> GetLayout() const { return m_Layout; }
		inline std::vector<Ref<Mesh>> GetMeshes() const { return m_Meshes; }

		inline unsigned int GetTotalVertices() const { return total_vertices; }
		inline unsigned int GetTotalIndices() const { return total_indices; }
		inline const std::vector<oglm::vec2<unsigned int>> GetUpdate();

		std::vector<Ref<Mesh>>::iterator begin() { return m_Meshes.begin(); }
		std::vector< Ref<Mesh>>::iterator end() { return m_Meshes.end(); }
		std::vector< Ref<Mesh>>::reverse_iterator rbegin() { return m_Meshes.rbegin(); }
		std::vector< Ref<Mesh>>::reverse_iterator rend() { return m_Meshes.rend(); }

		std::vector<Ref<Mesh>>::const_iterator begin() const { return m_Meshes.begin(); }
		std::vector<Ref<Mesh>>::const_iterator end() const { return m_Meshes.end(); }
		std::vector<Ref<Mesh>>::const_reverse_iterator rbegin() const { return m_Meshes.rbegin(); }
		std::vector< Ref<Mesh>>::const_reverse_iterator rend() const { return m_Meshes.rend(); }

		inline const float const* GetVertexBuffer() const { return vertexbuffer.get(); }
		inline const unsigned int const* GetIndexBuffer() const { return indexbuffer.get(); }
	private:
		std::vector<Ref<Mesh>> m_Meshes;
		Scope<float[]> vertexbuffer;
		Scope<unsigned int[]> indexbuffer;
		Ref<VertexBufferLayout> m_Layout = CreateRef<VertexBufferLayout>();
		unsigned int total_vertices = 0, total_indices = 0;
	};
}