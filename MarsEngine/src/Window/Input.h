#pragma once

#include "MarsFlags.h"

#include "Vender/MTL/Ref.h"
#include "Vender/glm/glm/glm.hpp"

namespace ME 
{
	namespace Window
	{
		class Input
		{
		public:
			static const bool& IsKeyPressed(const int& keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

			static const bool& IsMouseMoved() { return s_Instance->GetMouseMovedImpl(); }
			static const bool& IsMousePressed(const int& mousecode) { return s_Instance->IsMousePressedImpl(mousecode); }
			static const float& GetMouseX() { return s_Instance->GetMouseXImpl(); }
			static const float& GetMouseY() { return s_Instance->GetMouseYImpl(); }
			static const glm::vec2& GetMouse() { return s_Instance->GetMouseImpl(); }
			static const glm::vec2& GetMouseDelta() { return s_Instance->GetMouseDeltaImpl(); }
			static const float& GetMouseDeltaX() { return s_Instance->GetMouseDeltaXImpl(); }
			static const float& GetMouseDeltaY() { return s_Instance->GetMouseDeltaYImpl(); }

			static Input* Get() { return s_Instance; }
			static Input* Create();

		protected:
			virtual const bool& IsKeyPressedImpl(const int& keycode) = 0;

			virtual const bool& GetMouseMovedImpl() = 0;
			virtual const bool& IsMousePressedImpl(const int& keycode) = 0;
			virtual const float& GetMouseXImpl() = 0;
			virtual const float& GetMouseYImpl() = 0;
			virtual const glm::vec2& GetMouseDeltaImpl() = 0;
			virtual const float& GetMouseDeltaXImpl() = 0;
			virtual const float& GetMouseDeltaYImpl() = 0;
			virtual const glm::vec2& GetMouseImpl() = 0;
		private:
			static Input* s_Instance;
		};
	}
}