#include "MarsHeader.h"
#include "OrthographicCameraController.h"

#include "Window/Input.h"
#include "Window/Events/KeyboardCode.h"
#include "Window/Events/MouseEvent.h"
#include "Window/Events/UIEvent.h"

#include "Vender/glm/glm/gtc/matrix_transform.hpp"

namespace ME
{
	OrthographicCameraController::OrthographicCameraController(const glm::vec2& size)
		:Layer("OrthographicCameraController"), m_Size(size), m_CameraTranslationSpeed(100.0f), 
		m_CameraRotationSpeed(100.0f), m_Camera(-size.x/2.0f, size.x/2.0f, -size.y/2.0f, size.y/2.0f) 
	{
		m_Camera.changed = true;
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Window::Input::IsKeyPressed(Event::Key::A))
		{
			m_Camera.m_Postion.x -= m_CameraTranslationSpeed * ts;
			m_Camera.changed = true;
		}
		if (Window::Input::IsKeyPressed(Event::Key::D))
		{
			m_Camera.m_Postion.x += m_CameraTranslationSpeed * ts;
			m_Camera.changed = true;
		}
		if (Window::Input::IsKeyPressed(Event::Key::W))
		{
			m_Camera.m_Postion.y += m_CameraTranslationSpeed * ts;
			m_Camera.changed = true;
		}
		if (Window::Input::IsKeyPressed(Event::Key::S))
		{
			m_Camera.m_Postion.y -= m_CameraTranslationSpeed * ts;
			m_Camera.changed = true;
		}

		if (Window::Input::IsKeyPressed(Event::Key::Q))
		{
			m_Camera.m_Rotate -= m_CameraRotationSpeed * ts;
			m_Camera.changed = true;
		}
		if (Window::Input::IsKeyPressed(Event::Key::E))
		{
			m_Camera.m_Rotate += m_CameraRotationSpeed * ts;
			m_Camera.changed = true;
		}
		m_Camera.OnUpdate();
	}
	void OrthographicCameraController::OnEvent(Event::Event& e)
	{
		Event::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Event::MouseEvent::MouseScrolledEvent>(std::bind(&OrthographicCameraController::OnMouseScrolled, this, std::placeholders::_1));
		dispatcher.Dispatch<Event::UIWindowResizeEvent>(std::bind(&OrthographicCameraController::OnWindowResized, this, std::placeholders::_1));
	}
	bool OrthographicCameraController::OnMouseScrolled(Event::Event& e)
	{
		m_Camera.m_Zoom -= ((Event::MouseEvent::MouseScrolledEvent&)e).GetOffsetY();
		m_Camera.changed = true;
		return true;
	}
	bool OrthographicCameraController::OnWindowResized(Event::Event& e)
	{
		Event::UIWindowResizeEvent& event = (Event::UIWindowResizeEvent&)e;
		if (strcmp(event.getUIWindow()->getUIWindowTitle().c_str(), "Editor") == 0)
		{
			float X = event.getContentSize().x / 2.0f;
			float Y = event.getContentSize().y / 2.0f;
			m_Camera.m_Projection = glm::ortho(-X, X, -Y, Y, -1.0f, 1.0f);
			m_Camera.changed = true;
		}
		return true;
	}
}
