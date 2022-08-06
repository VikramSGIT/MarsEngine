#include "MarsHeader.h"
#include "WindowsInput.h"
#include "Vender/GLFW/glfw3.h"

namespace ME
{
	namespace Window
	{
		bool WindowsInput::IsKeyPressedImpl(const int& keycode) { return m_InputData.m_Keystack.find(keycode) != m_InputData.m_Keystack.end(); }
		bool WindowsInput::GetMouseMovedImpl() { return GetMouseDeltaImpl() != glm::vec2(0.0f, 0.0f); }
		bool WindowsInput::IsMousePressedImpl(const int& mousecode) { return m_InputData.m_Mousestack.find(mousecode) != m_InputData.m_Mousestack.end(); }
		float WindowsInput::GetMouseXImpl() { return m_InputData.m_MousePos.x; }
		float WindowsInput::GetMouseYImpl() { return m_InputData.m_MousePos.y; }
		glm::vec2 WindowsInput::GetMouseImpl() { return m_InputData.m_MousePos; }
		float WindowsInput::GetMouseDeltaXImpl() { return m_InputData.m_MouseDelta.x; }
		float WindowsInput::GetMouseDeltaYImpl() { return m_InputData.m_MouseDelta.y; }
		glm::vec2 WindowsInput::GetMouseDeltaImpl() { return m_InputData.m_MouseDelta; }
	}
}