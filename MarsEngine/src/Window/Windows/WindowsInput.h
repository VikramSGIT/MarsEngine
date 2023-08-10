#pragma once

#include "MarsFlags.h"

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
			virtual const bool& IsKeyPressedImpl(const int& keycode) override;

			virtual const bool& GetMouseMovedImpl() override;
			virtual const bool& IsMousePressedImpl(const int& mousecode) override;
			virtual const glm::vec2& GetMouseImpl() override;
			virtual const float& GetMouseXImpl() override;
			virtual const float& GetMouseYImpl() override;
			virtual const glm::vec2& GetMouseDeltaImpl() override;
			virtual const float& GetMouseDeltaXImpl() override;
			virtual const float& GetMouseDeltaYImpl() override;
		private:
			InputData m_InputData;
		};
	}
}