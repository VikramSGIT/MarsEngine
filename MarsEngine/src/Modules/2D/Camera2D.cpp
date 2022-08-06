#include "Camera2D.h"

namespace ME
{
	Camera2D::Camera2D(const glm::vec2& lb)
		:m_Transform(glm::identity<glm::mat4>()), validated(false), m_Zoom(1.0f), m_ViewPortSize(lb),
		m_Pan(0.0f, 0.0f), m_Rotate(0.0f, 0.0f), senZoom(0.1f), senPan(1.0f), senRot(1.0f), At(0.0f, 0.0f) {}

	void Camera2D::SetViewport(const glm::vec2& lb)
	{
		m_ViewPortSize = lb;
		validated = false;
	}
	void Camera2D::Zoom(const ME_DATATYPE& zoom, const glm::vec2& pos)
	{
		At = pos + m_Pan;
		m_Zoom += zoom * senZoom;
		validated = false;
	}
	void Camera2D::Pan(const glm::vec2& pan)
	{
		m_Pan += pan * senPan;
		validated = false;
	}
	void Camera2D::Rotate(const glm::vec2& rot)
	{
		m_Rotate += rot * senRot;
		validated = false;
	}
	void Camera2D::ZoomTo(const ME_DATATYPE& zoom, const glm::vec2& pos)
	{
		m_Zoom = zoom * senZoom;
		At = pos + m_Pan;
		validated = false;
	}
	void Camera2D::PanTo(const glm::vec2& pan)
	{
		m_Pan = pan * senPan;
		validated = false;
	}
	void Camera2D::RotateTo(const glm::vec2& rot)
	{
		m_Rotate = rot * senRot;
		validated = false;
	}
	Camera2D::operator glm::mat4()
	{
		Validate();
		return m_Transform;
	}
	void Camera2D::Reset() 
	{ 
		m_Pan = { 0.0f, 0.0f }, m_Rotate = { 0.0f, 0.0f }, m_Zoom = 1.0f, At = { 0.0f, 0.0f };
		m_Transform = glm::ortho(0.0f, m_ViewPortSize.x, m_ViewPortSize.y,  0.0f, -1.0f, 1.0f);
		validated = true;
	}
	void Camera2D::Validate()
	{
		if (validated)
			return;

		m_Transform =
			glm::translate(
				glm::scale(
					glm::translate(
						glm::ortho(m_Pan.x, m_ViewPortSize.x + m_Pan.x, m_ViewPortSize.y + m_Pan.y, m_Pan.y, -1.0f, 1.0f),
						glm::vec3(At, 0.0f)),
					glm::vec3(m_Zoom, m_Zoom, 0.0f)),
				glm::vec3(-At, 0.0f));

		validated = true;
	}
}

