#pragma once
#include "MarsHeader.h"

#include "Window/Window.h"
#include "WindowsInput.h"
#include "Window/Events/WindowEvent.h"
#include "Window/Events/MouseEvent.h"
#include "Window/Events/KeyEvent.h"

#include "Vender/GLFW/glfw3.h"
#include "Core/Logger.h"

#include <sstream>
#include <memory>
namespace ME
{
    namespace Window
    {
        namespace Windows
        {
            class WindowsWindow : public Window
            {
            public:
                WindowsWindow(const WindowProperty& props);
                virtual ~WindowsWindow();

                void OnUpdate() override;

                uint32_t GetWidth() const override { return m_Data.Width; }
                uint32_t GetHeight() const override { return m_Data.Height; }
                std::string GetTitle() const override { return m_Data.Title; }

                void SetEventCallBack(const EventCallBackFunc& callback) override { m_Data.fn = callback; }
                void SetVSync(bool enable) override;
                bool IsVSync() const override;

                virtual GLFWwindow* GetNativeWindow() override { return m_Window; }

            private:
                GLFWwindow* m_Window;

                virtual void Init(const WindowProperty& props);
                virtual void Shutdown();

                struct WindowData
                {
                    std::string Title;
                    uint32_t Width, Height;
                    InputData* Input;
                    bool VSync;

                    EventCallBackFunc fn;
                };

                WindowData m_Data;
            };
        }
    }
}