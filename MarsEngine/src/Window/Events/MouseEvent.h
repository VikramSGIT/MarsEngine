#ifndef ME_MOUSEEVENT
#define ME_MOUSEEVENT

#include "MarsHeader.h"

#include "Window/Events/Event.h"
#include "Window/Events/MouseCode.h"

#include "Vender/glm/glm/glm.hpp"
#include <string>
#include <sstream>
namespace ME
{
    namespace Event
    {
        namespace MouseEvent
        {
            class MouseMovedEvent : public Event
            {
            private:
                double m_MouseX, m_MouseY;
            public:
                MouseMovedEvent(const double x, const double y)
                    :m_MouseX(x), m_MouseY(y)
                {

                    ME_PROFILE_TRACE_CALL();

                }
                const double GetX() const { return m_MouseX; }
                const double GetY() const { return m_MouseY; }
                const glm::vec2 GetPos() const { return { m_MouseX, m_MouseY }; }

#ifdef ME_DEBUG_SHOW_EVENT
                std::string ToString() const override
                {
                    std::stringstream ss;
                    ss << "MouseMoveEvent: " << m_MouseX << ", " << m_MouseY;
                    return ss.str();
                }
#endif

                EVENT_CLASS_TYPE(MouseMoved)
                    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
            };

            class MouseScrolledEvent : public Event
            {
            private:
                double m_OffsetX, m_OffsetY;
            public:
                MouseScrolledEvent(const double offsetX, const double offsetY)
                    :m_OffsetX(offsetX), m_OffsetY(offsetY)
                {

                    ME_PROFILE_TRACE_CALL();

                }

                const double GetOffsetX() const { return m_OffsetX; }
                const double GetOffsetY() const { return m_OffsetY; }

#ifdef ME_DEBUG_SHOW_EVENT
                std::string ToString() const override
                {
                    std::stringstream ss;
                    ss << "MouseScrolledEvent: " << m_OffsetX << ", " << m_OffsetY;
                    return ss.str();
                }
#endif
                EVENT_CLASS_TYPE(MouseScrolled)
                    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
            };

            class MouseButtonEvent : public Event
            {
            protected:
                MouseButtonEvent(const MouseCode mousecode)
                    :m_MouseCode(mousecode) {}

                MouseCode m_MouseCode;

            public:

                MouseCode GetMouseCode() const { return m_MouseCode; }

                EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
            };

            class MouseButtonPressedEvent : public MouseButtonEvent
            {
            public:
                MouseButtonPressedEvent(const MouseCode mousecode)
                    :MouseButtonEvent(mousecode)
                {

                    ME_PROFILE_TRACE_CALL();

                }
#ifdef ME_DEBUG_SHOW_EVENT
                std::string ToString() const override
                {
                    std::stringstream ss;
                    ss << "MouseButtonPressed: " << m_MouseCode;
                    return ss.str();
                }
#endif
                EVENT_CLASS_TYPE(MouseButtonPressed)
            };

            class MouseButtonReleasedEvent : public MouseButtonEvent
            {
            public:
                MouseButtonReleasedEvent(const MouseCode mousecode)
                    :MouseButtonEvent(mousecode)
                {

                    ME_PROFILE_TRACE_CALL();

                }
#ifdef ME_DEBUG_SHOW_EVENT  
                std::string ToString() const override
                {
                    std::stringstream ss;
                    ss << "MouseButtonReleasedEvent: " << m_MouseCode;
                    return ss.str();
                }
#endif
                EVENT_CLASS_TYPE(MouseButtonReleased)
            };
        }
    }
}

#endif