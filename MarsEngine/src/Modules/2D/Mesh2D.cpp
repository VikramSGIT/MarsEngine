#include "MarsHeader.h"
#include "Mesh2D.h"

/*
* TODO:  Fix Reset Vertex, makes copy of vertex everytime
* ERROR: While try to update Mesh2D with new set of vertices (BufferVertices)
*/

namespace ME
{
//////////////////////////////////////// Mesh2D //////////////////////////////////////////////////

	Mesh2D::Mesh2D(const Mesh2D& mesh)
		:m_Name(mesh.m_Name), m_MeshData(mesh.m_MeshData), Ready(true) {}

	Mesh2D::Mesh2D(Mesh2D&& mesh) noexcept
		:m_Name(mesh.m_Name), m_MeshData(mesh.m_MeshData), Ready(true) {}

	Mesh2D::~Mesh2D()
	{
		dealloc(m_MeshData.vertex.vertex, m_MeshData.vertex.m_Size * sizeof(VERTEX2D));
		dealloc(m_MeshData.vertex.reset_vertex, m_MeshData.vertex.m_Size * sizeof(VERTEX2D));
		dealloc(m_MeshData.index.index, m_MeshData.index.m_Size* sizeof(unsigned int));
	}

	void Mesh2D::BufferVertices(const VERTEX2D* vertex, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		dealloc(m_MeshData.vertex.vertex, m_MeshData.vertex.m_Size);
		dealloc(m_MeshData.vertex.reset_vertex, m_MeshData.vertex.m_Size);

		m_MeshData.vertex.vertex = allocarr<VERTEX2D>(count);
		m_MeshData.vertex.m_Size = count;
		m_MeshData.vertex.reset_vertex = allocarr<VERTEX2D>(count);

		memcpy(m_MeshData.vertex.begin(), vertex, sizeof(VERTEX2D) * count);
		memcpy(m_MeshData.vertex.reset_vertex, vertex, sizeof(VERTEX2D) * count);
	}

	void Mesh2D::BufferIndices(const unsigned int* data, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		dealloc(m_MeshData.index.begin(), m_MeshData.index.m_Size);

		m_MeshData.index.index = allocarr<unsigned int>(count);
		m_MeshData.index.m_Size = count;

		memcpy(m_MeshData.index.begin(), data, sizeof(unsigned int) * count);
	}

	void Mesh2D::SetReset(const VERTEX2D* vertex)
	{

		ME_PROFILE_TRACE_CALL();

		memcpy(m_MeshData.vertex.begin(), vertex, sizeof(VERTEX2D) * m_MeshData.vertex.m_Size);
	}

	void Mesh2D::SetReset(const std::vector<VERTEX2D>& vertex)
	{

		ME_PROFILE_TRACE_CALL();

		memcpy(m_MeshData.vertex.begin(), &vertex[0], sizeof(VERTEX2D) * vertex.size());
	}

	void Mesh2D::Reset()
	{

		ME_PROFILE_TRACE_CALL();

		memcpy(m_MeshData.vertex.begin(), m_MeshData.vertex.reset_vertex, sizeof(VERTEX2D) * m_MeshData.vertex.m_Size);

		callback(this, m_MeshData.vertex.m_Offset);
		Ready = false;

	}

	void Mesh2D::Transform(const glm::mat4& matrix)
	{

		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), { -GetCentroid(), 1 });
		for (VERTEX2D& vertex : m_MeshData.vertex)
		{
			glm::vec4 out = { vertex.vertices[0], vertex.vertices[1], 1 , 1.0f };
			out = trans * out;
			out = matrix * out;
			out = glm::inverse(trans) * out;

			vertex.vertices[0] = out.x;
			vertex.vertices[1] = out.y;
		}

		callback(this, m_MeshData.vertex.m_Offset);
		Ready = false;

	}

	void Mesh2D::Translate(const glm::vec2& XYZ)
	{

		ME_PROFILE_TRACE_CALL();

		for (VERTEX2D& vertex : m_MeshData.vertex)
		{
			vertex.vertices[0] += XYZ.x;
			vertex.vertices[1] += XYZ.y;
		}

		callback(this, m_MeshData.vertex.m_Offset);
		Ready = false;

	}

	void Mesh2D::TranslateTo(const glm::vec2& XYZ)
	{

		ME_PROFILE_TRACE_CALL();

		glm::vec2 centroid = GetCentroid();

		glm::vec2 distance = XYZ - centroid;

		for (VERTEX2D& vertex : m_MeshData.vertex)
		{
			vertex.vertices[0] += distance.x;
			vertex.vertices[1] += distance.y;
		}

		callback(this, m_MeshData.vertex.m_Offset);
		Ready = false;

	}

	void Mesh2D::Rotate(const ME_DATATYPE& degree)
	{

		ME_PROFILE_TRACE_CALL();

		glm::mat4 origin = glm::translate(glm::identity<glm::mat4>(), { -GetCentroid(), 1 });

		for (VERTEX2D& vertex : m_MeshData.vertex)
		{
			glm::vec4 out(vertex.vertices[0], vertex.vertices[1], 1, 1);

			out = origin * out;
			out = glm::rotate(glm::identity<glm::mat4>(), glm::radians(degree), glm::vec3(0, 0, 1)) * out;
			out = glm::inverse(origin) * out;

			vertex.vertices[0] = out.x;
			vertex.vertices[1] = out.y;
		}

		callback(this, m_MeshData.vertex.m_Offset);
		Ready = false;

	}

	void Mesh2D::Scale(const glm::vec2& XY)
	{
		ME_PROFILE_TRACE_CALL();

		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), { -GetCentroid(), 1 });
		for (VERTEX2D& vertex : m_MeshData.vertex)
		{
			glm::vec4 out = { vertex.vertices[0], vertex.vertices[1], 1.0f,  1.0f };
			out = trans * out;
			out = glm::scale(glm::identity<glm::mat4>(), glm::vec3(XY.x, XY.y, 1)) * out;
			out = glm::inverse(trans) * out;

			vertex.vertices[0] = out.x;
			vertex.vertices[1] = out.y;
		}

		callback(this, m_MeshData.vertex.m_Offset);
		Ready = false;

	}

	const glm::vec2 Mesh2D::GetCentroid() const
	{

		ME_PROFILE_TRACE_CALL();

		glm::vec2 Result = { 0.0f, 0.0f };
		for (const VERTEX2D& vertex : m_MeshData.vertex)
		{
			Result.x += vertex.vertices[0];
			Result.y += vertex.vertices[1];
		}
		Result.x = Result.x / static_cast<ME_DATATYPE>(m_MeshData.vertex.m_Size);
		Result.y = Result.y / static_cast<ME_DATATYPE>(m_MeshData.vertex.m_Size);
		return Result;
	}

////////////////////////////////////////// Mesh Queue 2D ////////////////////////////////////////////////

	MeshQueue2D::MeshQueue2D()
		:m_Layout(CreateRef<Renderer::VertexBufferLayout>()), total_indices(0), total_vertices(0)
	{
		if (m_Layout->GetTotalCount() <= 0)
		{
			m_Layout->push(GL_FLOAT, 2);
			m_Layout->push(GL_FLOAT, 2);
			m_Layout->push(GL_FLOAT, 1);
		}
	}

	void MeshQueue2D::PushMesh(const Ref<Mesh2D>& mesh)
	{

		ME_PROFILE_TRACE_CALL();

		Ref<Mesh2D> m = mesh;
		m->callback = std::bind(&MeshQueue2D::MeshCallback, this, std::placeholders::_1, std::placeholders::_2);
		m_Meshes.emplace_back(mesh);
		total_vertices += mesh->GetMeshData().vertex.Size() * m_Layout->GetTotalCount();
		total_indices += mesh->GetMeshData().index.Size();
		m->Ready = true;
	}

	void MeshQueue2D::PushMeshes(const Vector<Ref<Mesh2D>>& meshes)
	{

		ME_PROFILE_TRACE_CALL();

		for (Ref<Mesh2D> m : meshes)
		{
			m->callback = std::bind(&MeshQueue2D::MeshCallback, this, std::placeholders::_1, std::placeholders::_2);
			m_Meshes.emplace_back(m);
			total_vertices += m->GetMeshData().vertex.Size() * m_Layout->GetTotalCount();
			total_indices += m->GetMeshData().index.Size();
			m->Ready = true;
		}

	}

	void MeshQueue2D::PushMeshes(const std::vector<Ref<Mesh2D>>& meshes)
	{

		ME_PROFILE_TRACE_CALL();

		for (Ref<Mesh2D> m : meshes)
		{
			m->callback = std::bind(&MeshQueue2D::MeshCallback, this, std::placeholders::_1, std::placeholders::_2);
			m_Meshes.emplace_back(m);
			total_vertices += m->GetMeshData().vertex.Size() * m_Layout->GetTotalCount();
			total_indices += m->GetMeshData().index.Size();
			m->Ready = true;
		}

	}

	void MeshQueue2D::PushAddon(ME::Addon::MeshAddon2D& addon)
	{

		ME_PROFILE_TRACE_CALL();

		for (Ref<Mesh2D> m : addon.GetMeshes())
		{
			m->callback = std::bind(&MeshQueue2D::MeshCallback, this, std::placeholders::_1, std::placeholders::_2);
			m_Meshes.emplace_back(m);
			total_vertices += m->GetMeshData().vertex.Size();
			total_indices += m->GetMeshData().index.Size();
			m->Ready = true;
		}
	}
}