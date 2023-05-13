#pragma once

#include "MarsFlags.h"

#include "Event.h"
#include "Window/UI.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Vender/glm/glm/glm.hpp"
namespace ME
{
	namespace Event
	{

		class UIWindowEvent : public Event
		{
		public:
			UIWindowEvent(UIWindow* window)
				:m_UIWindow(window) {}

			inline UIWindow* getUIWindow() { return m_UIWindow; }
		private:
			UIWindow* m_UIWindow;
		};

		class UIWindowResizeEvent : public UIWindowEvent
		{
		public:
			UIWindowResizeEvent(UIWindow* window, glm::uvec2 windowsize, glm::uvec2 contentsize)
				:m_WindowSize(windowsize), m_ContentSize(contentsize), UIWindowEvent(window) {}

			inline const glm::uvec2& getWindowSize() { return m_WindowSize; }
			inline const glm::uvec2& getContentSize() { return m_ContentSize; }

			EVENT_CLASS_TYPE(UIWindowResize)
				EVENT_CLASS_CATEGORY(EventCategoryUIWindow)
			
		private:
			glm::uvec2 m_WindowSize, m_ContentSize;
		};

		class UIWindowMoveEvent : public UIWindowEvent
		{
		public:
			UIWindowMoveEvent(UIWindow* window, glm::uvec2 pos)
				:m_Position(pos), UIWindowEvent(window) {}

			inline const glm::uvec2& getPosition() { return m_Position; }

			EVENT_CLASS_TYPE(UIWindowMove)
				EVENT_CLASS_CATEGORY(EventCategoryUIWindow)
		private:
			glm::uvec2 m_Position;
		};

		class UIWindowFocusEvent : public UIWindowEvent
		{
		public:
			UIWindowFocusEvent(UIWindow* window)
				:UIWindowEvent(window) {}

			EVENT_CLASS_TYPE(UIWindowFocus)
				EVENT_CLASS_CATEGORY(EventCategoryUIWindow)
		};

		class UIWindowLostFocusEvent : public UIWindowEvent
		{
		public:
			UIWindowLostFocusEvent(UIWindow* window)
				:UIWindowEvent(window) {}

			EVENT_CLASS_TYPE(UIWindowLostFocus)
				EVENT_CLASS_CATEGORY(EventCategoryUIWindow)
		};

		class UIWindowHideEvent : public UIWindowEvent
		{
		public:
			UIWindowHideEvent(UIWindow* window)
				:UIWindowEvent(window) {}

			EVENT_CLASS_TYPE(UIWindowHide)
				EVENT_CLASS_CATEGORY(EventCategoryUIWindow)
		};

		class UIWindowShowEvent : public UIWindowEvent
		{
		public:
			UIWindowShowEvent(UIWindow* window)
				:UIWindowEvent(window) {}

			EVENT_CLASS_TYPE(UIWindowShow)
				EVENT_CLASS_CATEGORY(EventCategoryUIWindow)
		};
	}
}
