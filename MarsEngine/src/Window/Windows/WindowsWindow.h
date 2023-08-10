#pragma once

#include "Window/Window.h"
#include "WindowsInput.h"

#include "Vender/GLFW/glfw3.h"

#include "String.h"
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

                void OnIOUpdate() override;

                virtual void setWindowVSync(bool vsync) override;
                virtual void setWindowTitle(const string& name) override;
                virtual void setWindowSize(const glm::uvec2& size) override;
                virtual void setWindowPosition(const glm::uvec2& pos) override;
                virtual void setEventCallBack(const EventCallBackFunc& callback) override { m_Data.fn = callback; }

                virtual inline const bool& getWindowVSync() override { return m_Data.VSync; }
                virtual inline const bool& getWindowFocus() override { return m_Data.Focus; }
                virtual inline const glm::uvec2& getWindowSize() override { return m_Data.Size; }
                virtual inline const glm::uvec2& getWindowPosition() override { return m_Data.Position; }
                virtual inline const bool& getWindowMaximize() override { return m_Data.Maximize; }
                virtual inline const string& getWindowTitle() override { return m_Data.Title; }

                virtual GLFWwindow* getNativeWindow() override { return m_NativeWindow; }

            private:
                GLFWwindow* m_NativeWindow;

                virtual void Init(const WindowProperty& props);
                virtual void Shutdown();
                void PoolInputs();


                struct WindowData
                {
                    Window* Window;

                    bool VSync;
                    bool Focus;
                    bool Maximize;
                    bool MouseInside;
                    string Title;
                    InputData* Input;
                    glm::uvec2 Size;
                    glm::uvec2 Position;

                    EventCallBackFunc fn;
                } m_Data;
            };
        }
    }
}