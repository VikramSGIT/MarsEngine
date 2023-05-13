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
	private:
		bool OnMouseScrolled(Event::Event& e);
		bool OnWindowResized(Event::Event& e);
	private:
		ME_DATATYPE m_CameraTranslationSpeed, m_CameraRotationSpeed;
		glm::vec2 m_Size;
		OrthographicCamera m_Camera;
	};
}
