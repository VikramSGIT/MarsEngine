#ifndef ME_CAMERA
#define ME_CAMERA

#pragma once

#include "Vender/glm/glm/gtc/matrix_transform.hpp"

namespace ME
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& pv);
		~Camera();

		inline glm::mat4& GetProjectionViewMat() { return m_PV; }
		inline void SetProjectionViewMat(const glm::mat4& pv) { m_PV = pv; }

		operator glm::mat4() { return m_PV; }
	private:
		glm::mat4 m_PV;
	};
}

#endif