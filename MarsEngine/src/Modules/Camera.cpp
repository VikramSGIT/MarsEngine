#include "MarsHeader.h"
#include "Camera.h"

namespace ME
{
	Camera::Camera(const glm::mat4& pv)
		:m_PV(pv) {}

	Camera::~Camera()
	{

	}
}

