#include "Mesh.h"

namespace ME
{
//////////////////////////////////////// Mesh //////////////////////////////////////////////////

	void Mesh::BufferVertices(const VERTEX* vertex, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		m_Vertices.clear();
		m_ResetVertices.clear();
		for (unsigned int i = 0; i < count; i++)
		{
			m_Vertices.emplace_back(vertex[i]);
			m_ResetVertices.emplace_back(vertex[i]);
		}
		m_Ready = false;
	}

	void Mesh::BufferIndices(const unsigned int* data, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		m_Indices.clear();
		for (unsigned int i = 0; i < count; i++)
			m_Indices.emplace_back(data[i]);
		m_Ready = false;
	}

	void Mesh::UpdateVertices(const VERTEX* vertex, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		m_Vertices.clear();
		for (unsigned int i = 0; i < count; i++)
			m_Vertices.emplace_back(vertex[i]);
		m_Ready = false;
	}

	void Mesh::UpdateIndices(const unsigned int* data, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		m_Indices.clear();
		for (unsigned int i = 0; i < count; i++)
			m_Indices.emplace_back(data[i]);
		m_Ready = false;
	}

	void Mesh::SetReset(const VERTEX* vertex, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		m_ResetVertices.clear();
		for (unsigned int i = 0; i < count; i++)
			m_ResetVertices.emplace_back(vertex[i]);
	}

	void Mesh::SetReset(const std::vector<VERTEX>& vertex)
	{
		m_ResetVertices.clear();
		m_ResetVertices = vertex;
	}

	void Mesh::Reset()
	{

		ME_PROFILE_TRACE_CALL();

		m_Vertices = m_ResetVertices;
		m_Ready = false;
	}

	void Mesh::Transform(const glm::mat4& matrix)
	{
		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), -GetCentroid());
		for (VERTEX& vertex : m_Vertices)
		{
			glm::vec4 out = { vertex.vertices[0], vertex.vertices[1], vertex.vertices[2] , 1.0f };
			out = trans * out;
			out = matrix * out;
			out = glm::inverse(trans) * out;

			vertex.vertices[0] = out.x;
			vertex.vertices[1] = out.y;
			vertex.vertices[2] = out.z;
		}
		m_Ready = false;
	}

	void Mesh::Translate(const glm::vec3& XYZ)
	{

		ME_PROFILE_TRACE_CALL();

		for (VERTEX& vertex : m_Vertices)
		{
			vertex.vertices[0] += XYZ.x;
			vertex.vertices[1] += XYZ.y;
			vertex.vertices[2] += XYZ.z;
		}
		m_Ready = false;
	}

	void Mesh::TranslateTo(const glm::vec3& XYZ)
	{

		ME_PROFILE_TRACE_CALL();

		glm::vec3 centroid = GetCentroid();

		glm::vec3 distance = XYZ - centroid;

		for (VERTEX& vertex : m_Vertices)
		{
			vertex.vertices[0] += distance.x;
			vertex.vertices[1] += distance.y;
			vertex.vertices[2] += distance.z;
		}
		m_Ready = false;
	}

	void Mesh::Rotate(const glm::vec3& degreeXYZ)
	{
		ME_PROFILE_TRACE_CALL();

		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), -GetCentroid());
		for (VERTEX& vertex : m_Vertices)
		{
			glm::vec4 out = { vertex.vertices[0], vertex.vertices[1], vertex.vertices[2] , 1.0f };
			out = trans * out;
			out = glm::transpose(glm::rotate(glm::identity<glm::mat4>(), glm::radians(degreeXYZ.x), glm::vec3(1.0f, 0.0f, 0.0f))) * out;
			out = glm::transpose(glm::rotate(glm::identity<glm::mat4>(), glm::radians(degreeXYZ.y), glm::vec3(0.0f, 1.0f, 0.0f))) * out;
			out = glm::transpose(glm::rotate(glm::identity<glm::mat4>(), glm::radians(degreeXYZ.z), glm::vec3(0.0f, 0.0f, 1.0f))) * out;
			out = glm::inverse(trans) * out;

			vertex.vertices[0] = out.x;
			vertex.vertices[1] = out.y;
			vertex.vertices[2] = out.z;
		}
		m_Ready = false;
	}

	void Mesh::Scale(const glm::vec3& XYZ)
	{
		ME_PROFILE_TRACE_CALL();

		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), -GetCentroid());
		for (VERTEX& vertex : m_Vertices)
		{
			glm::vec4 out = { vertex.vertices[0], vertex.vertices[1], vertex.vertices[2] , 1.0f };
			out = trans * out;
			out = glm::scale(glm::identity<glm::mat4>(), glm::vec3(XYZ.x, XYZ.y, XYZ.z)) * out;
			out = glm::inverse(trans) * out;

			vertex.vertices[0] = out.x;
			vertex.vertices[1] = out.y;
			vertex.vertices[2] = out.z;
		}
		m_Ready = false;
	}

	glm::vec3 Mesh::GetCentroid() const
	{

		ME_PROFILE_TRACE_CALL();

		glm::vec3 Result = { 0.0f, 0.0f, 0.0f };
		for (VERTEX const& vertex : m_Vertices)
		{
			Result.x += vertex.vertices[0];
			Result.y += vertex.vertices[1];
			Result.z += vertex.vertices[2];
		}
		Result.x = Result.x / static_cast<ME_DATATYPE>(m_Vertices.size());
		Result.y = Result.y / static_cast<ME_DATATYPE>(m_Vertices.size());
		Result.z = Result.z / static_cast<ME_DATATYPE>(m_Vertices.size());
		return Result;
	}

////////////////////////////////////////// Mesh Queue ////////////////////////////////////////////////

	void MeshQueue::PushMesh(const Ref<Mesh>& mesh)
	{

		ME_PROFILE_TRACE_CALL();

		m_Meshes.emplace_back(mesh);
		ClearBuffer();

		if (m_Layout->GetTotalCount() <= 0)
		{
			m_Layout->push(GL_FLOAT, 3);
			m_Layout->push(GL_FLOAT, 2);
			m_Layout->push(GL_FLOAT, 1);
		}
//
// This makes the allocation for the vertexbuffer and indexbuffer easier
//

		total_vertices += static_cast<unsigned int>(mesh->m_Vertices.size()) * m_Layout->GetTotalCount();
		total_indices += static_cast<unsigned int>(mesh->m_Indices.size());

		vertexbuffer = vertexbufferallocator.allocate(total_vertices);
		indexbuffer = indexbufferallocator.allocate(total_indices);

		unsigned int voffset = 0, ioffset = 0, indexoffset = 0;
		for (Ref<Mesh> ms : m_Meshes)
		{
//
// Filling up of vertexbuffer with datas
//
			std::vector<VERTEX>& vertex = ms->m_Vertices;
			std::vector<unsigned int>& index = ms->m_Indices;
			for (unsigned __int64 i = 0; i < vertex.size() * m_Layout->GetTotalCount(); i+= m_Layout->GetTotalCount())
			{
				unsigned int offset = 0;

				vertexbuffer[i + (offset++) + voffset] = vertex.at(i/m_Layout->GetTotalCount()).vertices[0];
				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).vertices[1];
				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).vertices[2];

				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).texturecoord[0];
				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).texturecoord[1];

				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).index;
			}
			voffset += static_cast<unsigned int>(ms->m_Vertices.size()) * m_Layout->GetTotalCount();
//                  
// Filling up indexbuffer with data with maintaining offsets of indices
//
			for (unsigned __int64 j = 0; j < index.size(); j++)
			{
				indexbuffer[j + ioffset] = index.at(j) + indexoffset;
			}
			ioffset += static_cast<unsigned int>(index.size());
			indexoffset += *std::max_element(index.begin(), index.end()) + 1;
			ms->SetReady(true);
		}
	}

	void MeshQueue::PushMeshes(const std::vector<Ref<Mesh>>& meshes)
	{

		ME_PROFILE_TRACE_CALL();

		ClearBuffer();

		if (m_Layout->GetTotalCount() <= 0)
		{
			m_Layout->push(GL_FLOAT, 3);
			m_Layout->push(GL_FLOAT, 2);
			m_Layout->push(GL_FLOAT, 1);
		}

		for (Ref<Mesh> mesh : meshes)
		{
			m_Meshes.emplace_back(mesh);
//
// This makes the allocation for the vertexbuffer and indexbuffer easier
//

			total_vertices += static_cast<unsigned int>(mesh->m_Vertices.size()) * m_Layout->GetTotalCount();
			total_indices += static_cast<unsigned int>(mesh->m_Indices.size());
		}

		vertexbuffer = vertexbufferallocator.allocate(total_vertices);
		indexbuffer = indexbufferallocator.allocate(total_indices);

		unsigned int voffset = 0, ioffset = 0, indexoffset = 0;
		for (Ref<Mesh> ms : m_Meshes)
		{
//
// Filling up of vertexbuffer with datas
//
			std::vector<VERTEX>& vertex = ms->m_Vertices;
			std::vector<unsigned int>& index = ms->m_Indices;
			for (unsigned __int64 i = 0; i < vertex.size() * m_Layout->GetTotalCount(); i += m_Layout->GetTotalCount())
			{
				unsigned int offset = 0;

				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).vertices[0];
				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).vertices[1];
				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).vertices[2];

				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).texturecoord[0];
				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).texturecoord[1];

				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).index;
			}
			voffset += static_cast<unsigned int>(vertex.size()) * m_Layout->GetTotalCount();
//                  
// Filling up indexbuffer with data with maintaining offsets of indices
//
			for (unsigned __int64 j = 0; j < index.size(); j++)
			{
				indexbuffer[j + ioffset] = index.at(j) + indexoffset;
			}
			ioffset += static_cast<unsigned int>(index.size());
			indexoffset += *std::max_element(index.begin(), index.end()) + 1;
			ms->SetReady(true);
		}
	}

	std::vector<glm::uvec2> MeshQueue::GetUpdate()
	{

		ME_PROFILE_TRACE_CALL();

		std::vector<glm::uvec2> ranges;

		unsigned int offset = 0;
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			bool ans = !m_Meshes[i]->m_Ready && (m_Meshes[i]->m_Name.size() == 17);
			ans;
			glm::uvec2 input;
			std::vector<VERTEX>& vertex = m_Meshes[i]->m_Vertices;
			if (!m_Meshes[i]->IsReady())
			{
				input.x = offset;
				input.y = offset + static_cast<unsigned int>(vertex.size()) * m_Layout->GetTotalCount();
				ranges.emplace_back(input);
				for (unsigned __int64 j = 0; j < (vertex.size() * m_Layout->GetTotalCount()); j += m_Layout->GetTotalCount())
				{
					unsigned int __offset__ = 0;
					vertexbuffer[j + (__offset__++) + offset] = vertex.at(j / m_Layout->GetTotalCount()).vertices[0];
					vertexbuffer[j + (__offset__++) + offset] = vertex.at(j / m_Layout->GetTotalCount()).vertices[1];
					vertexbuffer[j + (__offset__++) + offset] = vertex.at(j / m_Layout->GetTotalCount()).vertices[2];

					vertexbuffer[j + (__offset__++) + offset] = vertex.at(j / m_Layout->GetTotalCount()).texturecoord[0];
					vertexbuffer[j + (__offset__++) + offset] = vertex.at(j / m_Layout->GetTotalCount()).texturecoord[1];

					vertexbuffer[j + (__offset__++) + offset] = vertex.at(j / m_Layout->GetTotalCount()).index;
				}
				m_Meshes[i]->SetReady(true);
			}
			offset += static_cast<unsigned int>(vertex.size()) * m_Layout->GetTotalCount();
		}

		return ranges;
	}

	void MeshQueue::ClearBuffer()
	{

		ME_PROFILE_TRACE_CALL();

		vertexbufferallocator.deallocate(vertexbuffer, total_vertices);
		indexbufferallocator.deallocate(indexbuffer, total_indices);
	}

	void MeshQueue::PushAddon(ME::Addon::MeshAddon& addon)
	{

		ME_PROFILE_TRACE_CALL();

		ClearBuffer();

		if (m_Layout->GetTotalCount() <= 0)
		{
			m_Layout->push(GL_FLOAT, 3);
			m_Layout->push(GL_FLOAT, 2);
			m_Layout->push(GL_FLOAT, 1);
		}

		std::vector<Ref<Mesh>> temp;
		temp = m_Meshes;
		m_Meshes.clear();
		for (Ref<Mesh> mesh : addon.GetMeshes())
		{
			
			m_Meshes.emplace_back(mesh);
//
// This makes the allocation for the vertexbuffer and indexbuffer easier
//
			total_vertices += static_cast<unsigned int>(mesh->m_Vertices.size()) * m_Layout->GetTotalCount();
			total_indices += static_cast<unsigned int>(mesh->m_Indices.size());
		}

		for (Ref<Mesh> mesh : temp)
		{
			m_Meshes.emplace_back(mesh);
		}

		vertexbuffer = vertexbufferallocator.allocate(total_vertices);
		indexbuffer = indexbufferallocator.allocate(total_indices);

		unsigned int voffset = 0, ioffset = 0, indexoffset = 0;
		for (Ref<Mesh> ms : m_Meshes)
		{
//
// Filling up of vertexbuffer with datas
//			
			std::vector<VERTEX>& vertex = ms->m_Vertices;
			std::vector<unsigned int>& index = ms->m_Indices;
			for (unsigned __int64 i = 0; i < vertex.size() * m_Layout->GetTotalCount(); i += m_Layout->GetTotalCount())
			{
				unsigned int offset = 0;

				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).vertices[0];
				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).vertices[1];
				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).vertices[2];

				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).texturecoord[0];
				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).texturecoord[1];

				vertexbuffer[i + (offset++) + voffset] = vertex.at(i / m_Layout->GetTotalCount()).index;
			}
			voffset += static_cast<unsigned int>(vertex.size()) * m_Layout->GetTotalCount();
//                  
// Filling up indexbuffer with data with maintaining offsets of indices
//
			for (unsigned __int64 j = 0; j < index.size(); j++)
			{
				indexbuffer[j + ioffset] = index.at(j) + indexoffset;
			}
			ioffset += static_cast<unsigned int>(index.size());
			indexoffset += *std::max_element(index.begin(), index.end()) + 1;
			ms->SetReady(true);
		}
	}
}