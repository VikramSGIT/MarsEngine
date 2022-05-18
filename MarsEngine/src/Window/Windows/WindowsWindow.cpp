#include "WindowsWindow.h"
#include "Vender\imgui\imgui.h"
namespace ME
{
    namespace Window
    {
        Window* Window::Create(const WindowProperty& winprop)
        {

            ME_PROFILE_TRACE_CALL();

            std::stringstream ss;
            ss << "Window Created!! Name: " << winprop.Title << " Dimension: " << winprop.Width << " X " << winprop.Height;
            ME_CORE_WARNING(ss.str());
            Input::Input::Create();

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

                m_Data.Title = props.Title;
                m_Data.Width = props.Width;
                m_Data.Height = props.Height;
                m_Data.Input = static_cast<WindowsInput*>(Input::Input::Get())->GetFrameData();

                if (s_GLFWWindowCount == 0)
                {
                    ME_CORE_CRITICAL(!glfwInit(), "Could not initialize GLFW!!");
                }

                {
                    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
                    ++s_GLFWWindowCount;
                }

                glfwMakeContextCurrent(m_Window);
                glfwSetWindowUserPointer(m_Window, &m_Data);
                SetVSync(true);
                //
                //CallBacks from glfw
                //
                glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        data.Width = width;
                        data.Height = height;

                        Event::AppEvent::WindowResizeEvent event(width, height);
                        data.fn(event);
                    });

                glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        Event::AppEvent::WindowClosedEvent event;
                        data.fn(event);
                    });

                glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
#ifdef ME_IMGUI
                        ImGuiIO& io = ImGui::GetIO();
                        if (!io.WantCaptureKeyboard)
#endif
                        {
                            switch (action)
                            {
                            case GLFW_PRESS:
                            {
                                Event::KeyEvent::KeyPressedEvent event(key, 0);
                                keyrepeatcount = 0;
                                data.Input->m_Keystack.insert(key);
                                data.fn(event);
                                break;
                            }
                            case GLFW_RELEASE:
                            {
                                Event::KeyEvent::KeyReleasedEvent event(key);
                                data.Input->m_Keystack.erase(data.Input->m_Keystack.find(key));
                                data.fn(event);
                                break;
                            }
                            case GLFW_REPEAT:
                            {
                                Event::KeyEvent::KeyPressedEvent event(key, keyrepeatcount);
                                data.Input->m_Keystack.insert(key);
                                keyrepeatcount++;
                                data.fn(event);
                                break;
                            }
                            }
                        }
                    });

                glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int mousecode, int action, int mods)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
#ifdef ME_IMGUI
                        ImGuiIO& io = ImGui::GetIO();
                        if (!io.WantCaptureKeyboard)
#endif
                        {
                            switch (action)
                            {
                            case GLFW_PRESS:
                            {
                                Event::Mouse::MouseButtonPressedEvent event(mousecode);
                                data.Input->m_Mousestack.insert(mousecode);
                                data.fn(event);
                                break;
                            }
                            case GLFW_RELEASE:
                            {
                                Event::Mouse::MouseButtonReleasedEvent event(mousecode);
                                data.Input->m_Keystack.erase(data.Input->m_Keystack.find(mousecode));
                                data.fn(event);
                                break;
                            }
                            }
                        }
                    });

                glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double X, double Y)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        Event::Mouse::MouseScrooledEvent event(X, Y);
                        data.fn(event);
                    });

                glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double X, double Y)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        Event::Mouse::MouseMovedEvent event(X, Y);
                        data.Input->m_MousePos = { X, Y };
                        data.fn(event);
                    });

            }

            void WindowsWindow::OnUpdate()
            {

                ME_PROFILE_TRACE_CALL();

                glfwPollEvents();
                glfwSwapBuffers(m_Window);
            }

            void WindowsWindow::Shutdown()
            {

                ME_PROFILE_TRACE_CALL();

                glfwDestroyWindow(m_Window);
                --s_GLFWWindowCount;

                if (s_GLFWWindowCount == 0)
                {
                    glfwTerminate();
                    delete Input::Input::Get();
                }
            }

            void WindowsWindow::SetVSync(bool enable)
            {

                ME_PROFILE_TRACE_CALL();

                if (enable)
                    glfwSwapInterval(1);
                else
                    glfwSwapInterval(0);

                m_Data.VSync = enable;
            }

            bool WindowsWindow::IsVSync() const
            {

                ME_PROFILE_TRACE_CALL();

                return m_Data.VSync;
            }
        }
    }
}