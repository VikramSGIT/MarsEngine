#include "Mesh.h"

namespace Renderer 
{
//////////////////////////////////////// Mesh //////////////////////////////////////////////////

	void Mesh::BufferVertices(const VERTEX* vertex, const unsigned int& count)
	{
		for (int i = 0; i < count; i++)
			m_Vertices.emplace_back(vertex[i]);
		Ready = false;
	}

	void Mesh::BufferIndices(const unsigned int* data, const unsigned int& count)
	{
		for (int i = 0; i < count; i++)
			m_Indices.emplace_back(data[i]);
		Ready = false;
	}

	void Mesh::UpdateVertices(const VERTEX* vertex, const unsigned int& count)
	{
		m_Vertices.clear();
		for (int i = 0; i < count; i++)
			m_Vertices.emplace_back(vertex[i]);
		Ready = false;
	}

	void Mesh::UpdateIndices(const unsigned int* data, const unsigned int& count)
	{
		m_Indices.clear();
		for (int i = 0; i < count; i++)
			m_Indices.emplace_back(data[i]);
		Ready = false;
	}

////////////////////////////////////////// Mesh Queue ////////////////////////////////////////////////

	void MeshQueue::PushMesh(Ref<Mesh> mesh)
	{
		m_Meshes.emplace_back(mesh);

		if (m_Layout->GetTotalCount() <= 0)
		{
			m_Layout->push(GL_FLOAT, 3);
			m_Layout->push(GL_FLOAT, 2);
			m_Layout->push(GL_FLOAT, 1);
		}
//
// This makes the allocation for the vertexbuffer and indexbuffer easier
// Need to add normals, oject index, vertex colors

		total_vertices += mesh->GetVertices()->size() * m_Layout->GetTotalCount();
		total_indices += mesh->GetIndices()->size();

		
		if (vertexbuffer != nullptr)
			delete[] vertexbuffer;
		if (indexbuffer != nullptr)
			delete[] indexbuffer;

		vertexbuffer = new ME_DATATYPE[total_vertices];
		indexbuffer = new unsigned int[total_indices];

		unsigned int voffset = 0, ioffset = 0, indexoffset = 0;
		for (Ref<Mesh> ms : m_Meshes)
		{
//
// Filling up of vertexbuffer with datas
//
			for (int i = 0; i < ms->GetVertices()->size() * m_Layout->GetTotalCount(); i+= m_Layout->GetTotalCount())
			{
				unsigned int offset = 0;

				vertexbuffer[i + (offset++) + voffset] = ms->GetVertices()->at(i/m_Layout->GetTotalCount()).vertices[0];
				vertexbuffer[i + (offset++) + voffset] = ms->GetVertices()->at(i / m_Layout->GetTotalCount()).vertices[1];
				vertexbuffer[i + (offset++) + voffset] = ms->GetVertices()->at(i / m_Layout->GetTotalCount()).vertices[2];

				vertexbuffer[i + (offset++) + voffset] = ms->GetVertices()->at(i / m_Layout->GetTotalCount()).texturecoord[0];
				vertexbuffer[i + (offset++) + voffset] = ms->GetVertices()->at(i / m_Layout->GetTotalCount()).texturecoord[1];

				vertexbuffer[i + (offset++) + voffset] = ms->GetVertices()->at(i / m_Layout->GetTotalCount()).index;
			}
			voffset += ms->GetVertices()->size() * m_Layout->GetTotalCount();
//                  
// Filling up indexbuffer with data with maintaining offsets of indices
//
			for (int j = 0; j < ms->GetIndices()->size(); j++)
			{
				indexbuffer[j + ioffset] = ms->GetIndices()->at(j) + indexoffset;
			}
			ioffset += ms->GetIndices()->size();
			indexoffset += *std::max_element(ms->GetIndices()->begin(), ms->GetIndices()->end()) + 1;
			ms->SetReady(true);
		}
	}

	void MeshQueue::ClearBuffer() const
	{
		if (vertexbuffer != nullptr)
			delete[] vertexbuffer;
		if (indexbuffer != nullptr)
			delete[] indexbuffer;
	}

	std::vector<oglm::vec2<unsigned int>> MeshQueue::GetUpdate()
	{
		std::vector<oglm::vec2<unsigned int>> ranges;

		unsigned int offset = 0;
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			oglm::vec2<unsigned int> input;
			if (!m_Meshes[i]->IsReady())
			{
				input.x = offset;
				input.y = offset + m_Meshes[i]->GetVertices()->size() * m_Layout->GetTotalCount();
				ranges.emplace_back(input);
				for (int j = offset; j < (m_Meshes[i]->GetVertices()->size() * m_Layout->GetTotalCount() + offset); j += m_Layout->GetTotalCount())
				{
					unsigned int __offset__ = 0;

					vertexbuffer[j + (__offset__++)] = m_Meshes[i]->GetVertices()->at(j / m_Layout->GetTotalCount()).vertices[0];
					vertexbuffer[j + (__offset__++)] = m_Meshes[i]->GetVertices()->at(j / m_Layout->GetTotalCount()).vertices[1];
					vertexbuffer[j + (__offset__++)] = m_Meshes[i]->GetVertices()->at(j / m_Layout->GetTotalCount()).vertices[2];

					vertexbuffer[j + (__offset__++)] = m_Meshes[i]->GetVertices()->at(j / m_Layout->GetTotalCount()).texturecoord[0];
					vertexbuffer[j + (__offset__++)] = m_Meshes[i]->GetVertices()->at(j / m_Layout->GetTotalCount()).texturecoord[1];

					vertexbuffer[j + (__offset__++)] = m_Meshes[i]->GetVertices()->at(j / m_Layout->GetTotalCount()).index;
				}
				m_Meshes[i]->SetReady(true);
			}
			else
				offset += m_Meshes[i]->GetVertices()->size() * m_Layout->GetTotalCount();
		}

		return ranges;
	}
}