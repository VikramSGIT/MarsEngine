#ifndef ME_INPUT
#define ME_INPUT

#pragma once

#include "MarsHeader.h"
#include "Core/Utilites/Ref.h"

#include "Vender/glm/glm/glm.hpp"

namespace ME 
{
	namespace Window
	{
		class Input
		{
		public:
			static bool IsKeyPressed(const int& keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

			static bool IsMouseMoved() { return s_Instance->GetMouseMovedImpl(); }
			static bool IsMousePressed(const int& mousecode) { return s_Instance->IsMousePressedImpl(mousecode); }
			static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
			static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
			static glm::vec2 GetMouse() { return s_Instance->GetMouseImpl(); }
			static glm::vec2 GetMouseDelta() { return s_Instance->GetMouseDeltaImpl(); }
			static float GetMouseDeltaX() { return s_Instance->GetMouseDeltaXImpl(); }
			static float GetMouseDeltaY() { return s_Instance->GetMouseDeltaYImpl(); }

			static Input* Get() { return s_Instance; }
			static Input* Create();

		protected:
			virtual bool IsKeyPressedImpl(const int& keycode) = 0;

			virtual bool GetMouseMovedImpl() = 0;
			virtual bool IsMousePressedImpl(const int& keycode) = 0;
			virtual float GetMouseXImpl() = 0;
			virtual float GetMouseYImpl() = 0;
			virtual glm::vec2 GetMouseDeltaImpl() = 0;
			virtual float GetMouseDeltaXImpl() = 0;
			virtual float GetMouseDeltaYImpl() = 0;
			virtual glm::vec2 GetMouseImpl() = 0;
		private:
			static Input* s_Instance;
		};
	}
}

#endif