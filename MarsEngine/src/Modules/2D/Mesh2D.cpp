#include "MarsHeader.h"
#include "Mesh2D.h"
#include "Core/Application.h"

#include "Vender/glm/glm/gtc/matrix_transform.hpp"
#include "Vender/glm/glm/glm.hpp"
/*
* TODO:  Fix Reset Vertex, makes copy of vertex everytime
* ERROR: While try to update Mesh2D with new set of vertices (BufferVertices)
*/

namespace ME
{
//////////////////////////////////////// Mesh2D //////////////////////////////////////////////////

	Mesh2D::Mesh2D(const string& name)
		:m_Name(name), m_MeshData(MeshData2D()) {}

	Mesh2D::Mesh2D(const Mesh2D& mesh)
		:m_Name(mesh.m_Name), m_MeshData(mesh.m_MeshData) {}

	Mesh2D::Mesh2D(Mesh2D&& mesh) noexcept
		:m_Name(mesh.m_Name), m_MeshData(mesh.m_MeshData) {}

	Mesh2D::~Mesh2D()
	{
		dealloc(m_MeshData.vertex.vertex);
		dealloc(m_MeshData.vertex.reset_vertex);
		dealloc(m_MeshData.index.index);
	}

	void Mesh2D::BufferVertices(const VERTEX2D* vertex, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		dealloc(m_MeshData.vertex.vertex);
		dealloc(m_MeshData.vertex.reset_vertex);

		m_MeshData.vertex.vertex = alloc<VERTEX2D>(count);
		m_MeshData.vertex.m_Size = count;
		m_MeshData.vertex.reset_vertex = alloc<VERTEX2D>(count);

		memcpy(m_MeshData.vertex.begin(), vertex, sizeof(VERTEX2D) * count);
		memcpy(m_MeshData.vertex.reset_vertex, vertex, sizeof(VERTEX2D) * count);
	}

	void Mesh2D::BufferIndices(const unsigned int* data, const unsigned int& count)
	{

		ME_PROFILE_TRACE_CALL();

		dealloc(m_MeshData.index.begin());

		m_MeshData.index.index = alloc<unsigned int>(count);
		m_MeshData.index.m_Size = count;

		memcpy(m_MeshData.index.begin(), data, sizeof(unsigned int) * count);
	}

	void Mesh2D::SetTexture(const Ref<Texture>& texture)
	{
		m_TextureData = texture;
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

	}

	void Mesh2D::Transform(const glm::mat4& matrix)
	{

		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), { -GetCentroid(), 1 });
		for (VERTEX2D& vertex : m_MeshData.vertex)
		{
			glm::vec4 out = { vertex.x, vertex.y, 1 , 1.0f };
			out = trans * out;
			out = matrix * out;
			out = glm::inverse(trans) * out;

			vertex.x = out.x;
			vertex.y = out.y;
		}
		Application::GetInstance().UpdateNotification(this);
	}

	void Mesh2D::Translate(const glm::vec2& XYZ)
	{

		ME_PROFILE_TRACE_CALL();

		for (VERTEX2D& vertex : m_MeshData.vertex)
		{
			vertex.x += XYZ.x;
			vertex.y += XYZ.y;
		}

		Application::GetInstance().UpdateNotification(this);
	}

	void Mesh2D::TranslateTo(const glm::vec2& XYZ)
	{

		ME_PROFILE_TRACE_CALL();

		glm::vec2 centroid = GetCentroid();

		glm::vec2 distance = XYZ - centroid;

		for (VERTEX2D& vertex : m_MeshData.vertex)
		{
			vertex.x += distance.x;
			vertex.y += distance.y;
		}

		Application::GetInstance().UpdateNotification(this);
	}

	void Mesh2D::Rotate(const ME_DATATYPE& degree)
	{

		ME_PROFILE_TRACE_CALL();

		glm::mat4 origin = glm::translate(glm::identity<glm::mat4>(), { -GetCentroid(), 1 });

		for (VERTEX2D& vertex : m_MeshData.vertex)
		{
			glm::vec4 out(vertex.x, vertex.y, 1, 1);

			out = origin * out;
			out = glm::rotate(glm::identity<glm::mat4>(), glm::radians(degree), glm::vec3(0, 0, 1)) * out;
			out = glm::inverse(origin) * out;

			vertex.x = out.x;
			vertex.y = out.y;
		}

		Application::GetInstance().UpdateNotification(this);
	}

	void Mesh2D::Scale(const glm::vec2& XY)
	{
		ME_PROFILE_TRACE_CALL();

		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), { -GetCentroid(), 1 });
		for (VERTEX2D& vertex : m_MeshData.vertex)
		{
			glm::vec4 out = { vertex.x, vertex.y, 1.0f,  1.0f };
			out = trans * out;
			out = glm::scale(glm::identity<glm::mat4>(), glm::vec3(XY.x, XY.y, 1)) * out;
			out = glm::inverse(trans) * out;

			vertex.x = out.x;
			vertex.y = out.y;
		}

		Application::GetInstance().UpdateNotification(this);
	}

	const glm::vec2 Mesh2D::GetCentroid() const
	{

		ME_PROFILE_TRACE_CALL();

		glm::vec2 Result = { 0.0f, 0.0f };
		for (const VERTEX2D& vertex : m_MeshData.vertex)
		{
			Result.x += vertex.x;
			Result.y += vertex.y;
		}
		Result.x = Result.x / static_cast<ME_DATATYPE>(m_MeshData.vertex.m_Size);
		Result.y = Result.y / static_cast<ME_DATATYPE>(m_MeshData.vertex.m_Size);
		return Result;
	}

	Mesh2D Mesh2D::operator*(const glm::mat4& mat)
	{
		ME_PROFILE_TRACE_CALL();

		glm::mat4 matrix = mat;
		for (VERTEX2D& vertex : m_MeshData.vertex)
		{
			glm::vec4 out(vertex.x, vertex.y, 1, 1.0f);
			out = matrix * out;
			vertex.x = out.x;
			vertex.y = out.y;
		}
		return *this;
		Application::GetInstance().UpdateNotification(this);
	}

//////////////////////////////////////////////  Rectangle /////////////////////////////////////////////////////////

	Rectangle::Rectangle(const string& name, const unsigned int& index)
		:Mesh2D(name)
	{
		VERTEX2D vertexbuffer[] =
		{
			{0.0f, 0.0f, 0.0f, 0.0f, (float)index},
			{1.0f, 0.0f, 1.0f, 0.0f, (float)index},
			{1.0f, 1.0f, 1.0f, 1.0f, (float)index},
			{0.0f, 1.0f, 0.0f, 1.0f, (float)index}
		};
		unsigned int indexbuffer[] =
		{
			0, 1, 2,
			2, 3, 0
		};
		BufferVertices(vertexbuffer, 4);
		BufferIndices(indexbuffer, 6);
	}
	Rectangle::Rectangle(const string& name, const glm::vec2& lb, const unsigned int& index)
		:Mesh2D(name)
	{

		ME_PROFILE_TRACE_CALL();

		VERTEX2D vertexbuffer[] =
		{
			{0.0f, 0.0f, 0.0f, 0.0f, (float)index},
			{lb.x, 0.0f, 1.0f, 0.0f, (float)index},
			{lb.x, lb.y, 1.0f, 1.0f, (float)index},
			{0.0f, lb.y, 0.0f, 1.0f, (float)index}
		};
		unsigned int indexbuffer[] =
		{
			0, 1, 2,
			2, 3, 0
		};
		BufferVertices(vertexbuffer, 4);
		BufferIndices(indexbuffer, 6);
	}
	Rectangle::Rectangle(const string& name, const glm::vec2& edge1, glm::vec2 edge2, const unsigned int& index)
		:Mesh2D(name)
	{

		ME_PROFILE_TRACE_CALL();

		VERTEX2D vertexbuffer[] =
		{
			{edge1.x, edge1.y, 0.0f, 0.0f, (float)index},
			{edge2.x, edge1.y, 1.0f, 0.0f, (float)index},
			{edge2.x, edge2.y, 1.0f, 1.0f, (float)index},
			{edge1.x, edge2.y, 0.0f, 1.0f, (float)index}
		};
		unsigned int indexbuffer[] =
		{
			0, 1, 2,
			2, 3, 0
		};
		BufferVertices(vertexbuffer, 4);
		BufferIndices(indexbuffer, 6);
		Application::GetInstance().UpdateNotification(this);
	}

	void Rectangle::Grow(const glm::vec2& lb)
	{
		auto data = GetMeshData();
		data.vertex.begin()[1].x += lb.x;
		data.vertex.begin()[2].x += lb.x;
		data.vertex.begin()[2].y += lb.y;
		data.vertex.begin()[3].y += lb.y;

		Application::GetInstance().UpdateNotification(this);
	}
	void Rectangle::Set(const glm::vec2& lb)
	{
		auto data = GetMeshData();
		data.vertex.begin()[1].x += lb.x;
		data.vertex.begin()[2].x += lb.x;
		data.vertex.begin()[2].y += lb.y;
		data.vertex.begin()[3].y += lb.y;

		Application::GetInstance().UpdateNotification(this);
	}
}