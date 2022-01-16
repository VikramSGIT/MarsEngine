#include "WindowsInput.h"

namespace ME
{
	namespace Window
	{
		namespace Input
		{
			Input* Input::s_Instance = nullptr;

			Input* Input::Create()
			{
				//if (s_Instance != nullptr)
					Input::s_Instance = new WindowsInput();

				return Input::s_Instance;
			}

			bool WindowsInput::IsKeyPressedImpl(const int& keycode)
			{
				return m_InputData.m_Keystack.find(keycode) != m_InputData.m_Keystack.end();
			}
			bool WindowsInput::IsMousePressedImpl(const int& mousecode)
			{
				return m_InputData.m_Mousestack.find(mousecode) != m_InputData.m_Mousestack.end();
			}
			float WindowsInput::GetMouseXImpl()
			{
				return m_InputData.m_MousePos.x;
			}
			float WindowsInput::GetMouseYImpl()
			{
				return m_InputData.m_MousePos.x;
			}
			glm::vec2 WindowsInput::GetMousePosImpl()
			{
				return m_InputData.m_MousePos;
			}
		}
	}
}