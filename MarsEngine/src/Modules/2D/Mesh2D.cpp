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
		:Entity(name, "Mesh2D") 
	{
		m_Primitive = CreateRef<MeshPrimitive>();
		m_MeshData = CreateRef<MeshData2D>();
	}

	void Mesh2D::BufferData(const vector<VERTEX2D>& vertex, const vector<uint32_t>& index)
	{
		m_Primitive->vertex = vertex;
		m_Primitive->index = index;

		for (const VERTEX2D& ver : vertex)
			m_MeshData->centroid += ver.Position / glm::vec2{vertex.size(), vertex.size()};
	}

	void Mesh2D::SetTexture(const Ref<Texture>& texture)
	{
		m_TextureData = texture;
	}

	void Mesh2D::Translate(const glm::vec2& XY)
	{

		ME_PROFILE_TRACE_CALL();

		m_MeshData->translation = XY;

		Application::GetInstance().UpdateNotification(this);
	}

	void Mesh2D::Rotate(const ME_DATATYPE& degree)
	{

		ME_PROFILE_TRACE_CALL();

		m_MeshData->rotation = glm::radians(degree);

		Application::GetInstance().UpdateNotification(this);
	}

	void Mesh2D::Scale(const glm::vec2& XY)
	{
		ME_PROFILE_TRACE_CALL();

		m_MeshData->scale = XY;

		Application::GetInstance().UpdateNotification(this);
	}

	const glm::vec2 Mesh2D::GetCentroid() const
	{
		return m_MeshData->centroid;
	}
}