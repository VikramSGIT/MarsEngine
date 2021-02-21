#pragma once
#include "MarsHeader.h"

#include "Events/Event.h"
#include "Events/KeyboardCode.h"
#include "Vender/GLFW/glfw3.h"

#include <string>
#include <functional>
#include <vector>
#include <algorithm>

namespace Window
{
    struct WindowProperty
    {
        public:
            std::string Title;
            uint32_t Width;
            uint32_t Height;

            WindowProperty(const std::string& title = "Mars Engine",
            uint32_t widht = 1280,
            uint32_t height = 720)
                : Title(title), Width(widht), Height(height) {}
    };


    class Window
    {
    public:
        using EventCallBackFunc = std::function<void(Event::Event&)>;
        Ref<std::vector<uint16_t>> m_keystack = CreateRef<std::vector<uint16_t>>();

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual std::string GetTitle() const = 0;

        virtual void SetEventCallBack(const EventCallBackFunc& callback) = 0;
        virtual void SetVSync(bool enable) = 0;
        virtual bool IsVSync() const = 0;

        virtual GLFWwindow* GetNativeWindow() = 0;

        virtual bool IsKeyPressed(Event::KeyCode code) const
        {
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
}
