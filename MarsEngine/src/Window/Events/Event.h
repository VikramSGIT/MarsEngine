#ifndef ME_EVENT
#define ME_EVENT

#pragma once

#include "MarsHeader.h"

#include <iostream>

#define BIT(X) (1<<X)

namespace ME
{
    namespace Event
    {
        enum class EventType
        {
            None = 0,
            WindowClose, WindowResize, WindowMove, WindowFocus, WindowLostFocus, WindowVSyncOn, WindowVSyncOff,
            WindowMinimize, WindowMaximize, WindowMouseEnter, WindowMouseLeave,
            AppTick, AppUpdate, AppRender,
            KeyPressed, KeyRepeat, KeyReleased, KeyTyped,
            UIWindowResize, UIWindowClose, UIWindowFocus, UIWindowLostFocus, UIWindowMove, UIWindowMouseEnter, 
            UIWindowMouseLeave, UIWindowHide, UIWindowShow,
            MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
        };

        enum EventCategoryFlag
        {
            None = 0,
            EventCategoryWindow = BIT(0),
            EventCategoryInput = BIT(1),
            EventCategoryKeyboard = BIT(2),
            EventCategoryMouse = BIT(3),
            EventCategoryMouseButton = BIT(4),
            EventCategoryUIWindow = BIT(5)
        };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::type;}\
                                  virtual EventType GetEventType() const override{return GetStaticType();}\
                                  virtual const char* GetName() const override{return #type;}

#define EVENT_CLASS_CATEGORY(catogory) virtual int GetCategoryFlags() const {return catogory;}                               

        class Event
        {
        public:
            virtual ~Event() = default;

            bool Handled = false;

            virtual EventType GetEventType() const = 0;
            virtual const char* GetName() const = 0;
            virtual int GetCategoryFlags() const = 0;

#ifdef ME_DEBUG_SHOW_EVENT
            virtual std::string ToString() const { return GetName(); }
#endif

            bool IsInCategory(const EventCategoryFlag& category) { return GetCategoryFlags() & category; }
            bool IsInType(const EventType& type) { return GetEventType() == type; }
        };

        class EventDispatcher
        {
        public:
            EventDispatcher(Event& event)
                :m_Event(event) {}

            template<typename T, typename F>
            bool Dispatch(const F& func)
            {
                if (m_Event.GetEventType() == T::GetStaticType() && !m_Event.Handled)
                   m_Event.Handled |= func(static_cast<T&>(m_Event));

                return true;
            }
        private:
            Event& m_Event;
        };
#ifdef ME_DEBUG_SHOW_EVENTSTR
        inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.GetName(); }
#endif

        void throwEvent(Event& e);
    }
}

#endif