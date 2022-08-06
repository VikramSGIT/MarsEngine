#ifndef ME_CAMERA
#define ME_CAMERA

#include "MarsHeader.h"

#include "Vender/glm/glm/gtc/matrix_transform.hpp"

namespace ME
{
	class Camera2D
	{
	public:
		Camera2D() = default;
		Camera2D(const glm::vec2& lb);

		void SetViewport(const glm::vec2& lb);
		void Zoom(const ME_DATATYPE& zoom, const glm::vec2& pos = glm::vec2(0.0f, 0.0f));
		void Pan(const glm::vec2& pan);
		void Rotate(const glm::vec2& rot);

		void ZoomTo(const ME_DATATYPE& zoom, const glm::vec2& pos = glm::vec2(0.0f, 0.0f));
		void PanTo(const glm::vec2& pan);
		void RotateTo(const glm::vec2& rot);

		void Reset();
		operator glm::mat4();
		inline const glm::mat4& GetProjectionViewMat()
		{
			Validate();
			return m_Transform;
		}
	private:

		void Validate();

		glm::mat4 m_Transform;
		glm::vec2 m_Pan, m_Rotate, m_ViewPortSize, At;
		ME_DATATYPE m_Zoom, senZoom, senPan, senRot;
		bool validated;
	};
}

#endif