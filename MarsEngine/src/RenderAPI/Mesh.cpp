#include "Mesh.h"

namespace Renderer 
{
//////////////////////////////////////// Mesh //////////////////////////////////////////////////

	Mesh::Mesh(const std::string& name)
		:m_Name(name)
	{
		m_Vertices.emplace_back(1.0, 1.0, 1.0);
	}

	void Mesh::BufferVertices(const float* data, const unsigned int& size)
	{
		m_Vertices.clear();
		for (int i = 0; i < size/4; i+=3)
			m_Vertices.emplace_back(data[i], data[i + 1], data[i + 2]);
	}

	void Mesh::BufferIndices(const unsigned int* data, const unsigned int& size)
	{
		m_Indices.clear();
		for (int i = 0; i < size/4; i++)
			m_Indices.emplace_back(data[i]);
	}

////////////////////////////////////////// Mesh Queue ////////////////////////////////////////////////

	void MeshQueue::PushMesh(Mesh mesh)
	{
		m_Meshes.push_back(mesh);
		m_Layout->push(GL_FLOAT, 3);
//
// This makes the allocation for the vertexbuffer and indexbuffer easier
// Need to add texture coordinate, normals, oject index, vertex colors
		total_vertices += mesh.GetVertices().size() * 3;
		total_indices += mesh.GetIndices().size();
	}
}