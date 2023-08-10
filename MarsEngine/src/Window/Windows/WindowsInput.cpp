#include "MarsHeader.h"
#include "WindowsInput.h"

namespace ME
{
	namespace Window
	{
		const bool& WindowsInput::IsKeyPressedImpl(const int& keycode) { return m_InputData.m_Keystack.find(keycode) != m_InputData.m_Keystack.end(); }

		const bool& WindowsInput::GetMouseMovedImpl() { return GetMouseDeltaImpl() != glm::vec2(0.0f, 0.0f); }
		const bool& WindowsInput::IsMousePressedImpl(const int& mousecode) { return m_InputData.m_Mousestack.find(mousecode) != m_InputData.m_Mousestack.end(); }
		const float& WindowsInput::GetMouseXImpl() { return m_InputData.m_MousePos.x; }
		const float& WindowsInput::GetMouseYImpl() { return m_InputData.m_MousePos.y; }
		const glm::vec2& WindowsInput::GetMouseImpl() { return m_InputData.m_MousePos; }
		const float& WindowsInput::GetMouseDeltaXImpl() { return m_InputData.m_MouseDelta.x; }
		const float& WindowsInput::GetMouseDeltaYImpl() { return m_InputData.m_MouseDelta.y; }
		const glm::vec2& WindowsInput::GetMouseDeltaImpl() { return m_InputData.m_MouseDelta; }
	}
}