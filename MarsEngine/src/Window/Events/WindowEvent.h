#pragma once
#include "MarsFlags.h"

#include "Event.h"
#include "Core/Logger.h"

#include "Window/Window.h"

#include <sstream>
#include <string>
namespace ME
{
    namespace Event
    {
        namespace WindowEvent
        {
            class WindowEvent : public Event
            {
            public:
                WindowEvent(Window::Window* window)
                    :m_NativeWindow(window) {}

                inline Window::Window* getWindow() { return m_NativeWindow; }

                EVENT_CLASS_TYPE(None)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
            private:
                Window::Window* m_NativeWindow;
            };

            class WindowResizeEvent : public WindowEvent
            {
            public:
                WindowResizeEvent(Window::Window* window,const glm::uvec2& size)
                    :m_Size(size), WindowEvent(window)
                {

                    ME_PROFILE_TRACE_CALL();

                }

                inline const glm::uvec2& getWindowSize() { return m_Size; }

#ifdef ME_DEBUG_SHOW_EVENT
                std::string ToString() const override
                {
                    std::stringstream ss;
                    ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
                    return ss.str();
                }
#endif

                EVENT_CLASS_TYPE(WindowResize)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
            private:
                glm::uvec2 m_Size;
            };

            class WindowMoveEvent : public WindowEvent
            {
            public:
                WindowMoveEvent(Window::Window* window,const glm::uvec2& pos)
                    :WindowEvent(window)
                {
                    ME_PROFILE_TRACE_CALL()
                }

                inline const glm::uvec2& getWindowPosition() { return m_Position; }

                EVENT_CLASS_TYPE(WindowMove)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
            private:
                glm::uvec2 m_Position;
            };

            class WindowCloseEvent : public WindowEvent
            {
            public:
                WindowCloseEvent(Window::Window* window)
                    :WindowEvent(window) {}

                EVENT_CLASS_TYPE(WindowClose)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
#ifdef ME_DEBUG_SHOW_EVENT
                    std::string ToString() const override
                {
                    std::stringstream ss;
                    ss << "WindowCloseEvent";
                    return ss.str();
                }
#endif
            };

            class WindowFocus : public WindowEvent
            {
            public:
                WindowFocus(Window::Window* window)
                    :WindowEvent(window) {}

                EVENT_CLASS_TYPE(WindowFocus)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
            };

            class WindowLostFocus : public WindowEvent
            {
            public:
                WindowLostFocus(Window::Window* window)
                    :WindowEvent(window) {}

                EVENT_CLASS_TYPE(WindowLostFocus)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
            };

            class WindowVSyncOnEvent : public WindowEvent
            {
            public:
                WindowVSyncOnEvent(Window::Window* window)
                    :WindowEvent(window) {}

                EVENT_CLASS_TYPE(WindowVSyncOn)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
            };

            class WindowVSyncOffEvent : public WindowEvent
            {
            public:
                WindowVSyncOffEvent(Window::Window* window)
                    :WindowEvent(window) {}

                EVENT_CLASS_TYPE(WindowVSyncOff)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
            };

            class WindowMinimizeEvent : public WindowEvent
            {
            public:
                WindowMinimizeEvent(Window::Window* window)
                    :WindowEvent(window) {}

                EVENT_CLASS_TYPE(WindowMinimize)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
            };

            class WindowMaximizeEvent : public WindowEvent
            {
            public:
                WindowMaximizeEvent(Window::Window* window)
                    :WindowEvent(window) {}

                EVENT_CLASS_TYPE(WindowMaximize)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
            };

            class WindowMouseEnterEvent : public WindowEvent
            {
            public:
                WindowMouseEnterEvent(Window::Window* window)
                    :WindowEvent(window) {}

                EVENT_CLASS_TYPE(WindowMouseEnter)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
            };

            class WindowMouseLeaveEvent : public WindowEvent
            {
            public:
                WindowMouseLeaveEvent(Window::Window* window)
                    :WindowEvent(window) {}

                EVENT_CLASS_TYPE(WindowMouseLeave)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
            };

// TODO: Check usefulness 13/05/2023
            class ApptickEvent : public Event
            {
            public:
                ApptickEvent() = default;

                EVENT_CLASS_TYPE(AppTick)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
#ifdef ME_DEBUG_SHOW_EVENT
                    std::string ToString() const override
                {
                    std::stringstream ss;
                    ss << "AppTickEvent";
                    return ss.str();
                }
#endif
            };

            class AppUpdateEvent : public Event
            {
            public:
                AppUpdateEvent() = default;

                EVENT_CLASS_TYPE(AppUpdate)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
#ifdef ME_DEBUG_SHOW_EVENT
                    std::string ToString() const override
                {
                    std::stringstream ss;
                    ss << "AppUpdateEvent";
                    return ss.str();
                }
#endif
            };

            class AppRenderEvent : public Event
            {
            public:
                AppRenderEvent() = default;

                EVENT_CLASS_TYPE(AppRender)
                    EVENT_CLASS_CATEGORY(EventCategoryWindow)
#ifdef ME_DEBUG_SHOW_EVENT
                    std::string ToString() const override
                {
                    std::stringstream ss;
                    ss << "AppRenderEvent";
                    return ss.str();
                }
#endif
            };
        }
    }
}