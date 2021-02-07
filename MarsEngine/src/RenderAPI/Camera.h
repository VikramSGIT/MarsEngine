#pragma once

#include "MarsHeader.h"

#include "Vender/glm/glm/gtc/matrix_transform.hpp"

namespace ME
{
	namespace Renderer
	{
		class Camera
		{
		public:
			Camera();
			~Camera();

			inline glm::mat4& GetProjectionViewMat() { return m_PV; }
			inline void SetProjectionViewMat(const glm::mat4& pv) { m_PV = pv; }
		private:
			glm::mat4 m_PV;
		};
	}
}
