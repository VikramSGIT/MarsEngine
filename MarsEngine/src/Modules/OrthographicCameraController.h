#pragma once

#include "MarsFlags.h"
#include "Window/Layers/Layer.h"
#include "OrthographicCamera.h"

#include "Vender/glm/glm/glm.hpp"

namespace ME
{
	class OrthographicCameraController : public Window::Layer
	{
	public:
		OrthographicCameraController(const glm::vec2& size);

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event::Event& e) override;
		virtual void OnAttach() override {}
		virtual void OnDetach() override {}
		virtual void OnDraw() override {}

		virtual OrthographicCamera& getCamera() { return m_Camera; }

		void setTranslationSpeed(const ME_DATATYPE& speed) { m_CameraTranslationSpeed = speed; }
		void setRotationSpeed(const ME_DATATYPE& speed) { m_CameraRotationSpeed = speed; }
		void setScaleSpeed(const ME_DATATYPE& speed) { m_CameraZoomSpeed = speed; }
		void setMinZoom(const ME_DATATYPE& minzoom) { m_CameraMinZoom = minzoom; }

		inline const ME_DATATYPE& getTranslationSpeed() { return m_CameraTranslationSpeed; }
		inline const ME_DATATYPE& getRotationSpeed() { return m_CameraRotationSpeed; }
		inline const ME_DATATYPE& getZoomSpeed() { return m_CameraZoomSpeed; }
		inline const ME_DATATYPE& getMinZoom() { return m_CameraMinZoom; }
	private:
		bool OnMouseScrolled(Event::Event& e);
		bool OnWindowResized(Event::Event& e);
	private:
		ME_DATATYPE m_CameraTranslationSpeed, m_CameraRotationSpeed, m_CameraZoomSpeed, m_CameraMinZoom;
		glm::vec2 m_Size;
		OrthographicCamera m_Camera;
	};
}
