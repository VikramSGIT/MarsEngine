#pragma once

#include "MarsFlags.h"

#include "Events/Event.h"

#include "Core/Utilites/String.h"
#include "Core/Utilites/Ref.h"
#include "Window/Layers/Layer.h"

#include "Vender/GLFW/glfw3.h"
#include "Vender/glm/glm/glm.hpp"
#include <functional>

namespace ME
{
    namespace Window
    {
        struct WindowProperty
        {
        public:
            string Title;
            glm::uvec2 Size{ 1280, 720 };
        };

        class Window
        {
        public:
            using EventCallBackFunc = std::function<void(Event::Event&)>;

            virtual ~Window() = default;

            virtual void OnIOUpdate() = 0;

            virtual void setWindowVSync(bool) = 0;
            virtual void setWindowTitle(const string&) = 0;
            virtual void setWindowSize(const glm::uvec2&) = 0;
            virtual void setWindowPosition(const glm::uvec2&) = 0;
            virtual void setEventCallBack(const EventCallBackFunc&) = 0;

            virtual inline const glm::uvec2& getWindowSize() = 0;
            virtual inline const bool& getWindowVSync() = 0;
            virtual inline const bool& getWindowFocus() = 0;
            virtual inline const string& getWindowTitle() = 0;
            virtual inline const glm::uvec2& getWindowPosition() = 0;
            virtual inline const bool& getWindowMaximize() = 0;

            virtual GLFWwindow* getNativeWindow() = 0;
            static Window* Create(const WindowProperty& winprop = WindowProperty());
        };

        class WindowIOLayer : public IOLayer
        {
        public:
            WindowIOLayer(Window* window)
                :m_Window(window) {}

            virtual void OnIOUpdate() override { m_Window->OnIOUpdate(); }

            inline Window& getWindow() { return *m_Window; }
        private:
            Window* m_Window;
        };
    }
}
