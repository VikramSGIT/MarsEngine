#ifndef ME_CAMERA
#define ME_CAMERA

#include "MarsHeader.h"

#include "Vender/glm/glm/gtc/matrix_transform.hpp"

namespace ME
{
	namespace Renderer
	{
		class Camera2D
		{
		public:
			Camera2D(const glm::mat4& pv);

			inline glm::mat4& GetProjectionViewMat() { return m_PV; }
			inline void SetProjectionViewMat(const glm::mat4& pv) { m_PV = pv; }
		private:
			glm::mat4 m_PV;
		};
	}
}

#endif