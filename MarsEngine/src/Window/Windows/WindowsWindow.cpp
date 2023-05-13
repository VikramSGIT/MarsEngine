#include "MarsHeader.h"
#include "WindowsWindow.h"

#include "Window/Events/WindowEvent.h"
#include "Window/Events/MouseEvent.h"
#include "Window/Events/KeyEvent.h"

#include "Vender\imgui\imgui.h"
namespace ME
{
    namespace Window
    {
        Window* Window::Create(const WindowProperty& winprop)
        {

            ME_PROFILE_TRACE_CALL();

            ME_CORE_WARNING("Window Created!! Name: {} Dimension: {} x {}", winprop.Title.c_str(), winprop.Size.x, winprop.Size.y);

            return new Windows::WindowsWindow(winprop);
        }
        namespace Windows
        {
            static uint8_t s_GLFWWindowCount = 0;
            uint8_t keyrepeatcount = 0;
            WindowsWindow::WindowsWindow(const WindowProperty& props)
            {

                ME_PROFILE_TRACE_CALL();

                Init(props);
            }

            WindowsWindow::~WindowsWindow()
            {

                ME_PROFILE_TRACE_CALL();

                Shutdown();
            }

            void WindowsWindow::Init(const WindowProperty& props)
            {

                ME_PROFILE_TRACE_CALL();

                m_Data.Window = this;
                m_Data.Title = props.Title;
                m_Data.Size = props.Size;
                m_Data.Input = ((WindowsInput*)(Input::Input::Get()))->GetFrameData();

                if (s_GLFWWindowCount == 0)
                {
                    ME_CORE_CRITICAL(!glfwInit(), "Could not initialize GLFW!!");
                }

                {
                    m_NativeWindow = glfwCreateWindow((int)props.Size.x, (int)props.Size.y, m_Data.Title.c_str(), nullptr, nullptr);
                    ++s_GLFWWindowCount;
                }

                glfwMakeContextCurrent(m_NativeWindow);
                glfwSetWindowUserPointer(m_NativeWindow, &m_Data);
                glfwSwapInterval(1);
                //
                //CallBacks from glfw
                //
                glfwSetWindowSizeCallback(m_NativeWindow, [](GLFWwindow* window, int width, int height)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        data.Size = glm::uvec2{ width, height };

                        Event::WindowEvent::WindowResizeEvent event(data.Window, glm::uvec2 {width, height});
                        data.fn(event);
                    });

                glfwSetWindowPosCallback(m_NativeWindow, [](GLFWwindow* window, int X, int Y)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        data.Position = glm::uvec2{ X, Y };
                        Event::WindowEvent::WindowMoveEvent event(data.Window, data.Position);
                        data.fn(event);
                    });

                glfwSetWindowFocusCallback(m_NativeWindow, [](GLFWwindow* window, int focus) 
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        if (focus == GLFW_TRUE)
                        {
                            data.Focus = true;
                            Event::WindowEvent::WindowFocus event(data.Window);
                            data.fn(event);
                        }
                        else
                        {
                            data.Focus = false;
                            Event::WindowEvent::WindowLostFocus event(data.Window);
                            data.fn(event);
                        }
                    });

                glfwSetWindowMaximizeCallback(m_NativeWindow, [](GLFWwindow* window, int maximize) 
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        if (maximize == GLFW_TRUE)
                        {
                            data.Maximize = true;
                            Event::WindowEvent::WindowMaximizeEvent event(data.Window);
                            data.fn(event);
                        }
                        else
                        {
                            data.Maximize = false;
                            Event::WindowEvent::WindowMinimizeEvent event(data.Window);
                            data.fn(event);
                        }
                    });

                glfwSetCursorEnterCallback(m_NativeWindow, [](GLFWwindow* window, int entered)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        if (entered == GLFW_TRUE)
                        {
                            data.MouseInside = true;
                            Event::WindowEvent::WindowMouseEnterEvent event(data.Window);
                            data.fn(event);
                        }
                        else
                        {
                            data.MouseInside = false;
                            Event::WindowEvent::WindowMouseLeaveEvent event(data.Window);
                            data.fn(event);
                        }
                    });

                glfwSetWindowCloseCallback(m_NativeWindow, [](GLFWwindow* window)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        Event::WindowEvent::WindowCloseEvent event(data.Window);
                        data.fn(event);
                    });

                glfwSetKeyCallback(m_NativeWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
#ifdef ME_IMGUI
                        ImGuiIO& io = ImGui::GetIO();
                        if (!io.WantTextInput)
#endif
                        {
                            switch (action)
                            {
                            case GLFW_PRESS:
                            {
                                Event::KeyEvent::KeyPressedEvent event(key);
                                keyrepeatcount = 0;
                                data.Input->m_Keystack.insert(key);
                                data.fn(event);
                                break;
                            }
                            case GLFW_RELEASE:
                            {
                                Event::KeyEvent::KeyReleasedEvent event(key);
                                data.Input->m_Keystack.erase(key);
                                break;
                            }
                            case GLFW_REPEAT:
                            {
                                keyrepeatcount++;
                                Event::KeyEvent::KeyRepeatEvent event(key, keyrepeatcount);
                                data.fn(event);
                                break;
                            }
                            }
                        }
                    });

                glfwSetMouseButtonCallback(m_NativeWindow, [](GLFWwindow* window, int mousecode, int action, int mods)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        switch (action)
                        {
                        case GLFW_PRESS:
                        {
                            Event::MouseEvent::MouseButtonPressedEvent event(mousecode);
                            data.Input->m_Mousestack.insert(mousecode);
                            data.fn(event);
                            break;
                        }
                        case GLFW_RELEASE:
                        {
                            Event::MouseEvent::MouseButtonReleasedEvent event(mousecode);
                            data.Input->m_Mousestack.erase(mousecode);
                            data.fn(event);
                            break;
                        }
                        }
                    });

                glfwSetScrollCallback(m_NativeWindow, [](GLFWwindow* window, double X, double Y)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        Event::MouseEvent::MouseScrolledEvent event(X, Y);
                        data.fn(event);
                    });

                glfwSetCursorPosCallback(m_NativeWindow, [](GLFWwindow* window, double X, double Y)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        Event::MouseEvent::MouseMovedEvent event(X, Y);
                        data.fn(event);
                    });

            }

            void WindowsWindow::OnIOUpdate()
            {

                ME_PROFILE_TRACE_CALL();

                PoolInputs();
                glfwPollEvents();
                glfwSwapBuffers(m_NativeWindow);
            }

            void WindowsWindow::setWindowVSync(bool vsync)
            {
                m_Data.VSync = vsync;
                if (m_Data.VSync)
                {
                    glfwSwapInterval(1);
                    Event::WindowEvent::WindowVSyncOnEvent event((Window*)m_NativeWindow);
                    m_Data.fn(event);
                }
                else
                {
                    glfwSwapInterval(0);
                    Event::WindowEvent::WindowVSyncOffEvent event((Window*)m_NativeWindow);
                    m_Data.fn(event);
                }
            }

            void WindowsWindow::setWindowTitle(const string& name)
            {
                glfwSetWindowTitle(m_NativeWindow, name.c_str());
            }

            void WindowsWindow::setWindowSize(const glm::uvec2& size)
            {
                glfwSetWindowSize(m_NativeWindow, size.x, size.y);
            }

            void WindowsWindow::setWindowPosition(const glm::uvec2& pos)
            {
                glfwSetWindowPos(m_NativeWindow, pos.x, pos.y);
            }

            void WindowsWindow::Shutdown()
            {

                ME_PROFILE_TRACE_CALL();

                glfwDestroyWindow(m_NativeWindow);
                --s_GLFWWindowCount;

                if (s_GLFWWindowCount == 0)
                {
                    glfwTerminate();
                }
            }

            void WindowsWindow::PoolInputs()
            {
                double X, Y;
                int wx, wy;
                glfwGetWindowPos(m_NativeWindow, &wx, &wy);
                glfwGetCursorPos(m_NativeWindow, &X, &Y);
                m_Data.Input->m_MouseDelta = m_Data.Input->m_MousePos - glm::vec2{ X + wx, Y + wy };
                m_Data.Input->m_MousePos = { X + wx, Y + wy };
            }
        }
    }
}