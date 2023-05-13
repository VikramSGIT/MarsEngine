#include "MarsHeader.h"

#include "OrthographicCamera.h"
#include "Vender/glm/glm/gtc/matrix_transform.hpp"

namespace ME
{
	OrthographicCamera::OrthographicCamera(const ME_DATATYPE& left, const ME_DATATYPE& right, const ME_DATATYPE& bottom, const ME_DATATYPE& top)
		:m_View(glm::identity<glm::mat4>()), changed(true), m_Zoom(1.0f), m_Postion(0.0f, 0.0f), m_Rotate(0.0f)
	{
		m_Projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjection = m_Projection;
	}

	void OrthographicCamera::setZoom(const ME_DATATYPE& zoom)
	{
		m_Zoom = zoom;
		changed = true;
	}
	void OrthographicCamera::setPosition(const glm::vec2& pos)
	{
		m_Postion = pos;
		changed = true;
	}
	void OrthographicCamera::setRotate(const ME_DATATYPE& rot)
	{
		m_Rotate = rot;
		changed = true;
	}
	OrthographicCamera::operator glm::mat4()
	{
		OnUpdate();
		return m_ViewProjection;
	}
	void OrthographicCamera::OnUpdate()
	{
		if (!changed) return;

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), glm::vec3(m_Postion, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(m_Zoom, m_Zoom, 1.0f)) *
			glm::rotate(glm::mat4(1.0f), m_Rotate, glm::vec3(0.0f, 0.0f, 1.0f));

		m_View = glm::inverse(transform);

		m_ViewProjection = m_Projection * m_View;
	}
}

