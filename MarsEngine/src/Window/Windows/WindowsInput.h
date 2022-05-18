#ifndef ME_WINDOWSINPUT
#define ME_WINDOWSINPUT

#include "MarsHeader.h"
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
			glm::vec2 m_MousePos;
		};

		class WindowsInput : public Input
		{
		public:
			InputData* GetFrameData() { return &m_InputData; }

		protected:
			virtual bool IsKeyPressedImpl(const int& keycode) override;

			virtual bool IsMousePressedImpl(const int& mousecode) override;
			virtual float GetMouseXImpl() override;
			virtual float GetMouseYImpl() override;
			virtual glm::vec2 GetMousePosImpl() override;
		private:
			InputData m_InputData;
		};
	}
}

#endif