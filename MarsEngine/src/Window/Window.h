#ifndef ME_WINDOW
#define ME_WINDOW

#include "MarsHeader.h"

#include "Events/Event.h"
#include "Events/KeyboardCode.h"
#include "Vender/GLFW/glfw3.h"

#include <string>
#include <functional>

namespace ME
{
    namespace Window
    {
        struct WindowProperty
        {
        public:
            std::string Title;
            uint32_t Width = 1280;
            uint32_t Height = 720;
        };


        class Window
        {
<<<<<<< HEAD
            auto it = std::find(m_keystack->begin(), m_keystack->end(), code);
            if (it != m_keystack->end())
                return true;
            return false;
        }
        virtual bool MouseButtonPressed(Event::KeyCode code) const
        {
            auto it = std::find(m_keystack->begin(), m_keystack->end(), code);
            if (it != m_keystack->end())
                return true;
            return false;
        }
        static Ref<Window> Create(const WindowProperty& winprop = WindowProperty());
    };
=======
        public:
            using EventCallBackFunc = std::function<void(Event::Event&)>;

            virtual ~Window() = default;

            virtual void OnUpdate() = 0;

            virtual uint32_t GetWidth() const = 0;
            virtual uint32_t GetHeight() const = 0;
            virtual std::string GetTitle() const = 0;

            virtual void SetEventCallBack(const EventCallBackFunc& callback) = 0;
            virtual void SetVSync(bool enable) = 0;
            virtual bool IsVSync() const = 0;

            virtual GLFWwindow* GetNativeWindow() = 0;
            static Window* Create(const WindowProperty& winprop = WindowProperty());
        };
    }
>>>>>>> origin/master
}

#endif