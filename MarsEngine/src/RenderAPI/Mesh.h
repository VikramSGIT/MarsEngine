#pragma once

#include "MarsHeader.h"

#include "Buffers.h"
#include "Shader.h"
#include "Vender/OGLM/oglm.h"
#include <memory>

namespace Renderer
{

	class Mesh
	{
	public:
		Mesh(const std::string& name);
		~Mesh() = default;

		void BufferVertices(const float* data, const unsigned int& size);
		void BufferIndices(const unsigned int* data, const unsigned int& size);

		inline std::vector<oglm::vec3<float>>& GetVertices() { return m_Vertices; }
		inline std::vector<unsigned int>& GetIndices() { return m_Indices; }

		inline oglm::Matrix4<float>& GetModelMat() { return m_ModelMat; }
		inline bool IsReady() const { return Ready; }

		friend Mesh operator * (Mesh &mesh, const oglm::Matrix4<float> &mat)
		{
			mesh.m_ModelMat = mesh.m_ModelMat * mat;
		}

	private:
		std::vector<oglm::vec3<float>> m_Vertices;
		std::vector<unsigned int> m_Indices;
		oglm::Matrix4<float> m_ModelMat;
		std::string m_Name;
		bool Ready = false;
	};

	class MeshQueue
	{
	public:
		void PushMesh(Mesh mesh);

		inline Ref<VertexBufferLayout> GetLayout() const { return m_Layout; }
		inline std::vector<Mesh> GetMeshes() const { return m_Meshes; }
		inline unsigned int GetTotalVertices() const { return total_vertices; }
		inline unsigned int GetTotalIndices() const { return total_indices; }

		std::vector<Mesh>::iterator begin() { return m_Meshes.begin(); }
		std::vector<Mesh>::iterator end() { return m_Meshes.end(); }
		std::vector<Mesh>::reverse_iterator rbegin() { return m_Meshes.rbegin(); }
		std::vector<Mesh>::reverse_iterator rend() { return m_Meshes.rend(); }

		std::vector<Mesh>::const_iterator begin() const { return m_Meshes.begin(); }
		std::vector<Mesh>::const_iterator end() const { return m_Meshes.end(); }
		std::vector<Mesh>::const_reverse_iterator rbegin() const { return m_Meshes.rbegin(); }
		std::vector<Mesh>::const_reverse_iterator rend() const { return m_Meshes.rend(); }
	private:
		std::vector<Mesh> m_Meshes;
		Ref<VertexBufferLayout> m_Layout = CreateRef<VertexBufferLayout>();
		unsigned int total_vertices = 0, total_indices = 0;
	};
}