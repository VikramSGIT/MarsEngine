#include "MarsHeader.h"
#include "Mesh.h"
#include "Core/Application.h"

#include "Vender/glm/glm/glm.hpp"
#include "Vender/glm/glm/gtc/matrix_transform.hpp"
#include <memory>

/*
* TODO:  Fix Reset Vertex, makes copy of vertex everytime
* ERROR: While try to update mesh with new set of vertices (BufferVertices)
*/

namespace ME
{
	//////////////////////////////////////// Mesh //////////////////////////////////////////////////

	Mesh::Mesh(const string& name)
		:m_MeshData(MeshData()), Entity(name, "Mesh") {}

	Mesh::Mesh(const Mesh& mesh)
		:m_MeshData(mesh.m_MeshData), Entity(mesh.getName(), "Mesh") {}

	Mesh::Mesh(Mesh&& mesh) noexcept
		:m_MeshData(mesh.m_MeshData), Entity(mesh.getName(), "Mesh") {}

	Mesh::~Mesh()
	{
		dealloc(m_MeshData.vertex.vertex);
		dealloc(m_MeshData.vertex.reset_vertex);
		dealloc(m_MeshData.index.index);
	}

	void Mesh::BufferVertices(const VERTEX* vertex, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		dealloc(m_MeshData.vertex.vertex);
		dealloc(m_MeshData.vertex.reset_vertex);

		m_MeshData.vertex.vertex = alloc<VERTEX>(count);
		m_MeshData.vertex.m_Size = count;
		m_MeshData.vertex.reset_vertex = alloc<VERTEX>(count);

		memcpy(m_MeshData.vertex.begin(), vertex, sizeof(VERTEX) * count);
		memcpy(m_MeshData.vertex.reset_vertex, vertex, sizeof(VERTEX) * count);
	}

	void Mesh::BufferIndices(const unsigned int* data, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		dealloc(m_MeshData.index.begin());

		m_MeshData.index.index = alloc<unsigned int>(count);
		m_MeshData.index.m_Size = count;

		memcpy(m_MeshData.index.begin(), data, sizeof(unsigned int) * count);
	}

	void Mesh::SetTexture(const Ref<Texture>& texture)
	{
		m_TextureData = texture;
	}

	void Mesh::SetReset(const VERTEX* vertex)
	{

		ME_PROFILE_TRACE_CALL();

		memcpy(m_MeshData.vertex.begin(), vertex, sizeof(VERTEX) * m_MeshData.vertex.m_Size);
	}

	void Mesh::SetReset(const vector<VERTEX>& vertex)
	{

		ME_PROFILE_TRACE_CALL();

		memcpy(m_MeshData.vertex.begin(), &vertex[0], sizeof(VERTEX) * vertex.size());
	}

	void Mesh::Reset()
	{

		ME_PROFILE_TRACE_CALL();

		memcpy(m_MeshData.vertex.begin(), m_MeshData.vertex.reset_vertex, sizeof(VERTEX) * m_MeshData.vertex.m_Size);

		Application::GetInstance().UpdateNotification(this);

	}

	void Mesh::Translate(const glm::vec3& xyz)
	{

		ME_PROFILE_TRACE_CALL();

		for (VERTEX& vertex : m_MeshData.vertex)
		{
			vertex.x += xyz.x;
			vertex.y += xyz.y;
			vertex.z += xyz.z;
		}

		Application::GetInstance().UpdateNotification(this);

	}

	void Mesh::Transform(const glm::mat4& matrix)
	{

		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), -getCentroid());
		for (VERTEX& vertex : m_MeshData.vertex)
		{
			glm::vec4 out = { vertex.x, vertex.y, vertex.z , 1.0f };
			out = trans * out;
			out = matrix * out;
			out = glm::inverse(trans) * out;

			vertex.x = out.x;
			vertex.y = out.y;
			vertex.z = out.z;
		}

		Application::GetInstance().UpdateNotification(this);

	}

	void Mesh::TranslateTo(const glm::vec3& XYZ)
	{

		ME_PROFILE_TRACE_CALL();

		glm::vec3 centroid = getCentroid();
		glm::vec3 distance = XYZ - centroid;

		for (VERTEX& vertex : m_MeshData.vertex)
		{
			vertex.x += distance.x;
			vertex.y += distance.y;
			vertex.z += distance.z;
		}

		Application::GetInstance().UpdateNotification(this);

	}

	void Mesh::Rotate(const glm::vec3& degreeXYZ)
	{

		ME_PROFILE_TRACE_CALL();

		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), -getCentroid());
		for (VERTEX& vertex : m_MeshData.vertex)
		{
			glm::vec4 out = { vertex.x, vertex.y, vertex.z, 1.0f };
			out = trans * out;
			out = glm::transpose(glm::rotate(glm::identity<glm::mat4>(), glm::radians(degreeXYZ.x), glm::vec3(1.0f, 0.0f, 0.0f))) * out;
			out = glm::transpose(glm::rotate(glm::identity<glm::mat4>(), glm::radians(degreeXYZ.y), glm::vec3(0.0f, 1.0f, 0.0f))) * out;
			out = glm::transpose(glm::rotate(glm::identity<glm::mat4>(), glm::radians(degreeXYZ.z), glm::vec3(0.0f, 0.0f, 1.0f))) * out;
			out = glm::inverse(trans) * out;

			vertex.x = out.x;
			vertex.y = out.y;
			vertex.z = out.z;
		}

		Application::GetInstance().UpdateNotification(this);

	}

	void Mesh::Scale(const glm::vec3& XYZ)
	{
		ME_PROFILE_TRACE_CALL();

		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), -getCentroid());
		for (VERTEX& vertex : m_MeshData.vertex)
		{
			glm::vec4 out = { vertex.x, vertex.y, vertex.z, 1.0f };
			out = trans * out;
			out = glm::scale(glm::identity<glm::mat4>(), glm::vec3(XYZ.x, XYZ.y, XYZ.z)) * out;
			out = glm::inverse(trans) * out;

			vertex.x = out.x;
			vertex.y = out.y;
			vertex.z = out.z;
		}

		Application::GetInstance().UpdateNotification(this);
	}

	glm::vec3 Mesh::getCentroid() const
	{

		ME_PROFILE_TRACE_CALL();

		glm::vec3 Result = { 0.0f, 0.0f, 0.0f };
		for (const VERTEX& vertex : m_MeshData.vertex)
		{
			Result.x += vertex.x;
			Result.y += vertex.y;
			Result.z += vertex.z;
		}
		Result.x = Result.x / static_cast<ME_DATATYPE>(m_MeshData.vertex.m_Size);
		Result.y = Result.y / static_cast<ME_DATATYPE>(m_MeshData.vertex.m_Size);
		Result.z = Result.z / static_cast<ME_DATATYPE>(m_MeshData.vertex.m_Size);
		return Result;
	}
	Mesh Mesh::operator*(const glm::mat4& mat)
	{
		ME_PROFILE_TRACE_CALL();

		glm::mat4 matrix = mat;
		for (VERTEX& vertex : m_MeshData.vertex)
		{
			glm::vec4 out(vertex.x, vertex.y, vertex.z, 1.0f);
			out = matrix * out;
			vertex.x = out.x;
			vertex.y = out.y;
			vertex.z = out.z;
		}
		return *this;

		Application::GetInstance().UpdateNotification(this);
	}
}