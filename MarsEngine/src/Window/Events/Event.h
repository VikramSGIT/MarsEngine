#pragma once
#include "MarsHeader.h"

#include <functional>
#include <iostream>

#define BIT(X) (1<<X)

namespace Event
{
    enum class EventType
    {
        None = 0,
        WindowClosed, WindowResized, WindowLostFocused, WindowMoved,WindowFocused,
        AppTick, AppUpdate, AppRender, 
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
   };

   enum EventCategoryFlag
    {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput       = BIT(1),
        EventCategoryKeyboard    = BIT(2),
        EventCategoryMouse       = BIT(3),
        EventCategoryMouseButton = BIT(4)
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

        virtual EventType GetEventType() const  = 0;
        virtual const char* GetName() const  = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual void* GetGenericData() const = 0;

        #ifdef ME_DEBUG_SHOW_EVENT
        virtual std::string ToString() const  {return GetName();}
        #endif

        bool IsInCategory(EventCategoryFlag category) {return GetCategoryFlags() & category; }

        void* genericdata = nullptr;
        void DeleteGenericData()
        {

            ME_PROFILE_TRACE_CALL();

            if (EventType::WindowClosed == GetEventType()) {}
            else if (EventType::WindowResized == GetEventType())
            {
                delete[](unsigned int*) genericdata;
            }
            else if (EventType::WindowLostFocused == GetEventType()) {}
            else if (EventType::WindowMoved == GetEventType()) {}
            else if (EventType::WindowFocused == GetEventType()) {}
            else if (EventType::AppTick == GetEventType()) {}
            else if (EventType::AppUpdate == GetEventType()) {}
            else if (EventType::AppRender == GetEventType()) {}
            else if (EventType::KeyPressed == GetEventType())
            {
                delete[](uint16_t*) genericdata;
            }
            else if (EventType::KeyReleased == GetEventType())
            {
                delete (uint16_t*)genericdata;
            }
            else if (EventType::KeyTyped == GetEventType())
            {
                delete (uint16_t*)genericdata;
            }
            else if (EventType::MouseButtonPressed == GetEventType())
            {
                delete (uint16_t*)genericdata;
            }
            else if (EventType::MouseButtonReleased == GetEventType())
            {
                delete (uint16_t*)genericdata;
            }
            else if (EventType::MouseMoved == GetEventType())
            {
                delete[](double*) genericdata;
            }
            else if (EventType::MouseScrolled == GetEventType())
            {
                delete[](double*) genericdata;
            }
        }
    };
    
    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            :m_Event(event){}

        template<typename T,typename F>
        bool Dispatch(const F& func)
        {
            if(m_Event.GetEventType() == T::GetStaticType())
                m_Event.Handled |= func(static_cast<T&>(m_Event));

            return true;
        }
        private:
            Event& m_Event;
    };

    #ifdef ME_DEBUG_SHOW_EVENTSTR
    inline std::ostream& operator<<(std::ostream& os, const Event& e){return os << e.ToString();}
    #endif

}