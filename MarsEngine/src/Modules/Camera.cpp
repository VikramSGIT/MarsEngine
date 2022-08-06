#include "MarsHeader.h"
#include "Camera.h"

#include "Vender/glm/glm/glm.hpp"
#include "Vender/glm/glm/gtc/matrix_transform.hpp"

namespace ME
{
	Camera::Camera(const glm::vec2& lb)
		:m_Transform(glm::identity<glm::mat4>()), validated(false), m_Zoom(1.0f), m_ViewPortSize(lb),
		m_Pan(0.0f, 0.0f, 0.0f), m_Rotate(0.0f, 0.0f, 0.0f), m_Looking(0.0f, 0.0f, 0.0f), senZoom(0.1f), senPan(1.0f) {}

	void Camera::SetViewport(const glm::vec2& lb)
	{
		m_ViewPortSize = lb;
		validated = false;
	}
	void Camera::Zoom(const ME_DATATYPE& zoom)
	{
		m_Zoom += zoom * senZoom;
		validated = false;
	}
	void Camera::Pan(const glm::vec3& pan) 
	{ 
		m_Pan += pan * senPan;
		validated = false;
	}
	void Camera::Rotate(const glm::vec3& rot)
	{
		m_Rotate += rot * senRot;
		validated = false;
	}
	void Camera::ZoomTo(const ME_DATATYPE& zoom)
	{ 
		m_Zoom = zoom * (zoom * senZoom + 1);
		validated = false;
	}
	void Camera::PanTo(const glm::vec3& pan) 
	{ 
		m_Pan = pan * senPan;
		validated = false;
	}
	void Camera::RotateTo(const glm::vec3& rot)
	{
		m_Rotate = rot * senRot;
		validated = false;
	}
	void Camera::LookAt(const glm::vec3& pos) 
	{ 
		m_Looking = pos;
		validated = false;
	}
	Camera::operator glm::mat4()
	{
		Validate();
		return m_Transform;
	}
	void Camera::Reset() { m_Transform = glm::identity<glm::mat4>(); }
	void Camera::Validate()
	{
		if (validated)
			return;

		m_Transform = glm::ortho((-m_ViewPortSize.x/2 + m_Pan.x) / m_Zoom, (m_ViewPortSize.x/2 + m_Pan.x) / m_Zoom,
			(-m_ViewPortSize.y/2 + m_Pan.y) / m_Zoom, (m_ViewPortSize.y/2 + m_Pan.y) / m_Zoom,
			-1.0f + m_Pan.z, 1.0f + m_Pan.z);

		validated = true;
	}
}