#include "Mesh.h"

/*
* TODO:  Fix Reset Vertex, makes copy of vertex everytime
* ERROR: While try to update mesh with new set of vertices (BufferVertices)
*/

namespace ME
{
//////////////////////////////////////// Mesh //////////////////////////////////////////////////

	Mesh::Mesh(const Mesh& mesh)
		:m_Name(mesh.m_Name), m_MeshData(mesh.m_MeshData), Ready(true) {}

	Mesh::Mesh(Mesh&& mesh) noexcept
		:m_Name(mesh.m_Name), m_MeshData(mesh.m_MeshData), Ready(true) {}

	Mesh::~Mesh()
	{
		dealloc(m_MeshData.vertex.vertex, m_MeshData.vertex.m_Size);
		dealloc(m_MeshData.vertex.reset_vertex, m_MeshData.vertex.m_Size);
		dealloc(m_MeshData.index.index, m_MeshData.index.m_Size);
	}

	void Mesh::BufferVertices(const VERTEX* vertex, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		dealloc(m_MeshData.vertex.vertex, m_MeshData.vertex.m_Size);
		dealloc(m_MeshData.vertex.reset_vertex, m_MeshData.vertex.m_Size);

		m_MeshData.vertex.vertex = alloc<VERTEX>(count);
		m_MeshData.vertex.m_Size = count;

		memcpy(m_MeshData.vertex.begin(), vertex, sizeof(VERTEX) * count);
	}

	void Mesh::BufferIndices(const unsigned int* data, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		dealloc(m_MeshData.index.begin(), m_MeshData.index.m_Size);

		m_MeshData.index.index = alloc<unsigned int>(count);
		m_MeshData.index.m_Size = count;

		memcpy(m_MeshData.index.begin(), data, sizeof(unsigned int) * count);
	}

	void Mesh::SetReset(const VERTEX* vertex, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		memcpy(m_MeshData.vertex.begin(), vertex, sizeof(VERTEX) * count);
	}

	void Mesh::SetReset(const std::vector<VERTEX>& vertex)
	{

		ME_PROFILE_TRACE_CALL();

		memcpy(m_MeshData.vertex.begin(), &vertex[0], sizeof(VERTEX) * vertex.size());
	}

	void Mesh::Reset()
	{

		ME_PROFILE_TRACE_CALL();

		memcpy(m_MeshData.vertex.begin(), m_MeshData.vertex.reset_vertex, sizeof(VERTEX) * m_MeshData.vertex.m_Size);

		callback(this, m_MeshData.vertex.m_Offset);
		Ready = false;

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

		callback(this, m_MeshData.vertex.m_Offset);
		Ready = false;

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

		callback(this, m_MeshData.vertex.m_Offset);
		Ready = false;

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

		callback(this, m_MeshData.vertex.m_Offset);
		Ready = false;

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

		callback(this, m_MeshData.vertex.m_Offset);
		Ready = false;

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

		callback(this, m_MeshData.vertex.m_Offset);
		Ready = false;

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
		:m_Layout(CreateRef<Renderer::VertexBufferLayout>()), total_indices(0), total_vertices(0)
	{
		if (m_Layout->GetTotalCount() <= 0)
		{
			m_Layout->push(GL_FLOAT, 3);
			m_Layout->push(GL_FLOAT, 2);
			m_Layout->push(GL_FLOAT, 1);
		}
	}

	void MeshQueue::PushMesh(const Ref<Mesh>& mesh)
	{

		ME_PROFILE_TRACE_CALL();

		Ref<Mesh> m = mesh;
		m->callback = std::bind(&MeshQueue::MeshCallback, this, std::placeholders::_1, std::placeholders::_2);
		m_Meshes.emplace_back(mesh);
		total_vertices = mesh->GetMeshData().vertex.Size() * m_Layout->GetTotalCount();
		total_indices = mesh->GetMeshData().index.Size();
		m->Ready = true;
	}

	void MeshQueue::PushMeshes(const std::vector<Ref<Mesh>>& meshes)
	{

		ME_PROFILE_TRACE_CALL();

		for (Ref<Mesh> m : meshes)
		{
			m->callback = std::bind(&MeshQueue::MeshCallback, this, std::placeholders::_1, std::placeholders::_2);
			m_Meshes.emplace_back(m);
			total_vertices = m->GetMeshData().vertex.Size() * m_Layout->GetTotalCount();
			total_indices = m->GetMeshData().index.Size();
			m->Ready = true;
		}

	}

	void MeshQueue::PushAddon(ME::Addon::MeshAddon& addon)
	{

		ME_PROFILE_TRACE_CALL();

		for (Ref<Mesh> m : addon.GetMeshes())
		{
			m->callback = std::bind(&MeshQueue::MeshCallback, this, std::placeholders::_1, std::placeholders::_2);
			m_Meshes.emplace_back(m);
			total_vertices = m->GetMeshData().vertex.Size();
			total_indices = m->GetMeshData().index.Size();
			m->Ready = true;
		}
	}
}