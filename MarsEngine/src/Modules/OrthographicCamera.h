#pragma once

#include "MarsFlags.h"

#include "Vender/glm/glm/glm.hpp"

namespace ME
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(const ME_DATATYPE& left, const ME_DATATYPE& right, const ME_DATATYPE& bottom, const ME_DATATYPE& top);

		void setPosition(const glm::vec2& pos);
		void setRotate(const ME_DATATYPE& rot);
		void setZoom(const ME_DATATYPE& zoom);

		operator glm::mat4();

		inline const glm::vec2& getPosition() { return m_Postion; }
		inline const ME_DATATYPE& getRotate() { return m_Rotate; }
		inline const ME_DATATYPE& getZoom() { return m_Zoom; }
		inline const glm::mat4& getViewProjectionMatrix() { return m_ViewProjection; }

		void OnUpdate();
	private:
		glm::mat4 m_View, m_Projection, m_ViewProjection;
		glm::vec2 m_Postion;
		ME_DATATYPE m_Zoom, m_Rotate;
		bool changed;

		friend class OrthographicCameraController;
	};
}