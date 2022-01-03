#include "Mesh.h"

namespace ME
{
//////////////////////////////////////// Mesh //////////////////////////////////////////////////

	Mesh::Mesh(const Mesh& mesh)
		:m_Name(mesh.m_Name), m_MeshData(mesh.m_MeshData) {}

	Mesh::Mesh(Mesh&& mesh) noexcept
		:m_Name(mesh.m_Name), m_MeshData(mesh.m_MeshData) {}

	Mesh::~Mesh()
	{
		dealloc(m_MeshData.vertex.vertex, m_MeshData.vertex.m_Size);
		dealloc(m_MeshData.vertex.reset_vertex, m_MeshData.vertex.m_Size);
		dealloc(m_MeshData.index.index, m_MeshData.index.m_Size);
	}

	void Mesh::BufferVertices(const VERTEX* vertex, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		if (count == m_MeshData.vertex.m_Size)
		{
			dealloc(m_MeshData.vertex.vertex, m_MeshData.vertex.m_Size);
			dealloc(m_MeshData.vertex.reset_vertex, m_MeshData.vertex.m_Size);

			m_MeshData.vertex.vertex = alloc<VERTEX>(count);
			m_MeshData.vertex.m_Size = count;
		}

		memcpy(m_MeshData.vertex.vertex, vertex, sizeof(VERTEX) * count);

		m_Ready = false;
	}

	void Mesh::BufferIndices(const unsigned int* data, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		if (count == m_MeshData.index.m_Size)
		{
			dealloc(m_MeshData.index.index, m_MeshData.index.m_Size);

			m_MeshData.index.index = alloc<unsigned int>(m_MeshData.index.m_Size);
			m_MeshData.index.m_Size = count;
		}

		memcpy(m_MeshData.index.index, data, sizeof(unsigned int) * count);

		m_Ready = false;
	}

	void Mesh::SetReset(const VERTEX* vertex, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		memcpy(m_MeshData.vertex.vertex, vertex, sizeof(VERTEX) * count);
	}

	void Mesh::Reset()
	{

		ME_PROFILE_TRACE_CALL();

		memcpy(m_MeshData.vertex.vertex, m_MeshData.vertex.reset_vertex, sizeof(VERTEX) * m_MeshData.vertex.m_Size);

		m_Ready = false;
	}

	void Mesh::Transform(const glm::mat4& matrix)
	{

		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), -GetCentroid());
		for (VERTEX& vertex : m_MeshData.vertex)
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

		for (VERTEX& vertex : m_MeshData.vertex)
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

		for (VERTEX& vertex : m_MeshData.vertex)
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
		for (VERTEX& vertex : m_MeshData.vertex)
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
		for (VERTEX& vertex : m_MeshData.vertex)
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

	const glm::vec3 Mesh::GetCentroid() const
	{

		ME_PROFILE_TRACE_CALL();

		glm::vec3 Result = { 0.0f, 0.0f, 0.0f };
		for (const VERTEX& vertex : m_MeshData.vertex)
		{
			Result.x += vertex.vertices[0];
			Result.y += vertex.vertices[1];
			Result.z += vertex.vertices[2];
		}
		Result.x = Result.x / static_cast<ME_DATATYPE>(m_MeshData.vertex.m_Size);
		Result.y = Result.y / static_cast<ME_DATATYPE>(m_MeshData.vertex.m_Size);
		Result.z = Result.z / static_cast<ME_DATATYPE>(m_MeshData.vertex.m_Size);
		return Result;
	}

////////////////////////////////////////// Mesh Queue ////////////////////////////////////////////////

	MeshQueue::MeshQueue()
		:m_Layout(CreateRef<Renderer::VertexBufferLayout>())
	{
		if (m_Layout->GetTotalCount() <= 0)
		{
			m_Layout->push(GL_FLOAT, 3);
			m_Layout->push(GL_FLOAT, 2);
			m_Layout->push(GL_FLOAT, 1);
		}
	}

	void StaticQueue::PushMesh(const Ref<Mesh>& mesh)
	{

		ME_PROFILE_TRACE_CALL();

		if (!mesh->m_Static)
		{
			m_Meshes.emplace_back(mesh);
			m_Ready = false;
		}
	}

	void StaticQueue::PushMeshes(const std::vector<Ref<Mesh>>& meshes)
	{

		ME_PROFILE_TRACE_CALL();

		for (Ref<Mesh> m : meshes)
			if (!m->m_Static)
			{
				m_Meshes.emplace_back(m);
				m_Ready = false;
			}
	}

	void StaticQueue::PushAddon(ME::Addon::MeshAddon& addon)
	{

		ME_PROFILE_TRACE_CALL();

		for(Ref<Mesh> m : addon.GetMeshes())
			if (!m->m_Static)
			{
				m_Meshes.emplace_back(m);
				m_Ready = false;
			}
	}

	std::vector<glm::vec<2, unsigned int>> StaticQueue::GetUpdate()
	{

	}

	void StaticQueue::Allocate()
	{
		ME_PROFILE_TRACE_CALL();

		dealloc(VertexHead, total_vertices);
		dealloc(IndexHead, total_indices);
		VertexHead = alloc<VERTEX>(total_vertices);
		IndexHead = alloc<unsigned int>(total_indices);

		unsigned int voffset = 0, ioffset = 0, indexoffset = 0;
		for (Ref<Mesh> ms : m_Meshes)
		{
//
// Filling up of vertexbuffer with datas
//
			std::memcpy(&VertexHead[voffset], ms->GetMeshData().vertex.vertex, sizeof(VERTEX) * ms->GetMeshData().vertex.m_Size);
			dealloc(ms->GetMeshData().vertex.vertex, ms->GetMeshData().vertex.m_Size);

			ms->m_MeshData.vertex.vertex = (VERTEX*)&VertexHead[voffset];
			voffset += static_cast<unsigned int>(ms->GetMeshData().vertex.m_Size) * GetLayout()->GetTotalCount();

//                  
// Filling up indexbuffer with data with maintaining offsets of indices
//
			unsigned int maxindex = 0;
			for (unsigned int j = 0; j < ms->GetMeshData().index.m_Size; j++)
			{
				IndexHead[j + ioffset] = ms->GetMeshData().index.index[j] + indexoffset;
				if (IndexHead[j + ioffset] > maxindex)
					maxindex = IndexHead[j + ioffset];
			}
			dealloc(ms->GetMeshData().index.index, ms->GetMeshData().index.m_Size);
			ms->m_MeshData.index.index = &IndexHead[ioffset];

			ioffset += ms->GetMeshData().index.m_Size;
			indexoffset += maxindex + 1;

			ms->SetReady(true);
		}
	}

	void DynamicQueue::PushMesh(const Ref<Mesh>& mesh)
	{
		 
	}
}