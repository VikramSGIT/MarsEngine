#pragma once
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

                    unsigned int* window = new unsigned int[2];
                    *window = width;
                    *(window + 1) = height;
                    genericdata = (void*)window;
                }

                unsigned int GetWidth() const { return m_Width; }
                unsigned int GetHeight() const { return m_Height; }
                virtual void* GetGenericData() const override { return genericdata; }

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
                virtual void* GetGenericData() const override { return genericdata; }

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
                virtual void* GetGenericData() const override { return genericdata; }

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
                virtual void* GetGenericData() const override { return genericdata; }

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
                virtual void* GetGenericData() const override { return genericdata; }

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