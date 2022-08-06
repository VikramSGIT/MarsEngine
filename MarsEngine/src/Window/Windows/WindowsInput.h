#ifndef ME_WINDOWSINPUT
#define ME_WINDOWSINPUT

#pragma once

#include "Window/Input.h"
#include "Window/Events/KeyboardCode.h"
#include "Window/Events/MouseCode.h"

#include <set>

namespace ME
{
	namespace Window
	{
		struct InputData
		{
			std::set<Event::KeyCode> m_Keystack;
			std::set<Event::MouseCode> m_Mousestack;
			glm::vec2 m_MousePos, m_MouseDelta;
		};

		class WindowsInput : public Input
		{
		public:
			InputData* GetFrameData() { return &m_InputData; }

		protected:
			virtual bool IsKeyPressedImpl(const int& keycode) override;

			virtual bool GetMouseMovedImpl() override;
			virtual bool IsMousePressedImpl(const int& mousecode) override;
			virtual glm::vec2 GetMouseImpl() override;
			virtual float GetMouseXImpl() override;
			virtual float GetMouseYImpl() override;
			virtual glm::vec2 GetMouseDeltaImpl() override;
			virtual float GetMouseDeltaXImpl() override;
			virtual float GetMouseDeltaYImpl() override;
		private:
			InputData m_InputData;
		};
	}
}

#endif