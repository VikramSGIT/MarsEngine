#pragma once

#include "MarsHeader.h"

namespace ME 
{
	namespace Window
	{
		class Input
		{
		public:
			static bool IsKeyPressed(const int& keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

			static bool IsMousePressed(const int& mousecode) { return s_Instance->IsMousePressedImpl(mousecode); }
			static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
			static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
			static glm::vec2 GetMousePos() { return s_Instance->GetMousePosImpl(); }

			static Input* Get() { return s_Instance; }
			static Input* Create();

		protected:
			virtual bool IsKeyPressedImpl(const int& keycode) = 0;

			virtual bool IsMousePressedImpl(const int& keycode) = 0;
			virtual float GetMouseXImpl() = 0;
			virtual float GetMouseYImpl() = 0;
			virtual glm::vec2 GetMousePosImpl() = 0;
		private:
			static Input* s_Instance;
		};
	}
}