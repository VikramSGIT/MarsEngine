#ifndef ME_CAMERA
#define ME_CAMERA

#pragma once
#define ME_DATATYPE float
#include "Vender/glm/glm/gtc/matrix_transform.hpp"

namespace ME
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::vec2& lb);

		void SetViewport(const glm::vec2& lb);
		void Zoom(const ME_DATATYPE& zoom);
		void Pan(const glm::vec3& pan);
		void Rotate(const glm::vec3& rot);

		void ZoomTo(const ME_DATATYPE& zoom);
		void PanTo(const glm::vec3& pan);
		void RotateTo(const glm::vec3& rot);
		void LookAt(const glm::vec3& pos);

		void Reset();

		inline void SetProjectionViewMat(const glm::mat4& m_PV) { m_Transform = m_PV; }
		operator glm::mat4();
		inline const glm::mat4& GetProjectionViewMat()
		{ 
			Validate();
			return m_Transform;
		}
	private:

		void Validate();

		glm::mat4 m_Transform;
		glm::vec3 m_Pan, m_Rotate, m_Looking;
		glm::vec2 m_ViewPortSize;
		ME_DATATYPE m_Zoom, senZoom, senPan, senRot;
		bool validated;
	};
}

#endif