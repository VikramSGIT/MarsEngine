#pragma once
#include "MarsHeader.h"

#include "Window/Events/Event.h"
#include "KeyboardCode.h"

#include <string>
#include<sstream>

namespace Event
{
    namespace KeyEvent 
    {
        class KeyEvent : public Event
        {
        public:
            KeyCode GetkeyCode() const {return m_KeyCode;}
            virtual void* GetGenericData() const override { return genericdata; }

            EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
        protected:
            KeyEvent(const KeyCode keycode)
                : m_KeyCode(keycode) {}
        
        KeyCode m_KeyCode;
        };

        class KeyPressedEvent : public KeyEvent
        {
            private:
                uint16_t m_RepeatCount;
            public:
                KeyPressedEvent(const KeyCode keycode, const uint16_t repeatcount)
                    : KeyEvent(keycode), m_RepeatCount(repeatcount) 
                {
                        
                    ME_PROFILE_TRACE_CALL();

                    uint16_t* key = new uint16_t[2];
                    *key = keycode;
                    *(key + 1) = repeatcount;
                    genericdata = (void*)(key);
                }
                
                uint16_t GetRepeatCount() const { return m_RepeatCount;}

                #ifdef ME_DEBUG_SHOW_EVENT
                std::string ToString() const override
                {
                    std::stringstream ss;
                    ss << "KeyPressed: " << m_KeyCode << "(" << m_RepeatCount << ")";
                    return ss.str();
                }
                #endif
            EVENT_CLASS_TYPE(KeyPressed)
        };

        class KeyReleasedEvent : public KeyEvent
        {
        public:
            KeyReleasedEvent(const KeyCode keycode)
                : KeyEvent(keycode) 
                {
                    
                    ME_PROFILE_TRACE_CALL();

                    genericdata = (void*)(new KeyCode(keycode));
                }
            
            #ifdef ME_DEBUG_SHOW_EVENT
            std::string ToString() const override 
            {
                std::stringstream ss;
                ss << "KeyReleasedEvent: " << m_KeyCode;
                return ss.str();
            }
            #endif
        EVENT_CLASS_TYPE(KeyReleased)
        };

        class KeyTypedEvent : public KeyEvent
        {
        public:
            KeyTypedEvent(const KeyCode keycode)
                : KeyEvent(keycode) 
                {
                
                    ME_PROFILE_TRACE_CALL();

                    KeyCode key = keycode; 
                    genericdata = (void*)(new KeyCode(keycode));
                }
            #ifdef ME_DEBUG_SHOW_EVENT
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "KeyTyped: "<< m_KeyCode;
                return ss.str();
            }
            #endif
            EVENT_CLASS_TYPE(KeyTyped)
        };
    }
    
}