#ifndef ME_WINDOWSEVENT
#define ME_WINDOWSEVENT

#include "MarsHeader.h"

#include "Event.h"
#include "Core/Logger.h"

#include <sstream>
#include <string>
namespace ME
{
    namespace Event
    {
        namespace AppEvent
        {
            class WindowResizeEvent : public Event
            {
            private:
                unsigned int m_Width, m_Height;
            public:
                WindowResizeEvent(const unsigned int width, const unsigned int height)
                    :m_Width(width), m_Height(height)
                {

                    ME_PROFILE_TRACE_CALL();

                }

                unsigned int GetWidth() const { return m_Width; }
                unsigned int GetHeight() const { return m_Height; }
                glm::vec2 GetDimensions() const{ return {m_Width, m_Height}; }

#ifdef ME_DEBUG_SHOW_EVENT
                std::string ToString() const override
                {
                    std::stringstream ss;
                    ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
                    return ss.str();
                }
#endif

                EVENT_CLASS_TYPE(WindowResized)
                    EVENT_CLASS_CATEGORY(EventCategoryApplication)
            };

            class WindowClosedEvent : public Event
            {
            public:
                WindowClosedEvent() = default;

                EVENT_CLASS_TYPE(WindowClosed)
                    EVENT_CLASS_CATEGORY(EventCategoryApplication)
#ifdef ME_DEBUG_SHOW_EVENT
                    std::string ToString() const override
                {
                    std::stringstream ss;
                    ss << "WindowCloseEvent";
                    return ss.str();
                }
#endif
            };

            class ApptickEvent : public Event
            {
            public:
                ApptickEvent() = default;

                EVENT_CLASS_TYPE(AppTick)
                    EVENT_CLASS_CATEGORY(EventCategoryApplication)
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
                    EVENT_CLASS_CATEGORY(EventCategoryApplication)
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
                    EVENT_CLASS_CATEGORY(EventCategoryApplication)
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

#endif