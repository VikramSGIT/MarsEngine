#include "WindowsWindow.h"
#include "Vender\imgui\imgui.h"
namespace ME
{
<<<<<<< HEAD
    Ref<Window> Window::Create(const WindowProperty& winprop)
    {

        ME_PROFILE_TRACE_CALL();

        std::stringstream ss;
        ss << "Window Created!! Name: " << winprop.Title << " Dimension: " << winprop.Width << " X " << winprop.Height;
        ME_CORE_WARNING(ss.str());
        return  CreateRef<Windows::WindowsWindow>(winprop);
    }
    namespace Windows
    {
        static size_t s_GLFWWindowCount = 0;
        uint8_t keyrepeatcount = 0;
        WindowsWindow::WindowsWindow(const WindowProperty& props)
=======
    namespace Window
    {
        Window* Window::Create(const WindowProperty& winprop)
>>>>>>> origin/master
        {

            ME_PROFILE_TRACE_CALL();

            ME_CORE_WARNING("Window Created!! Name: {} Dimension: {} x {}", winprop.Title, winprop.Width, winprop.Height);
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
                m_Data.Input = ((WindowsInput*)(&*Input::Input::Get()))->GetFrameData();

                if (s_GLFWWindowCount == 0)
                {
                    ME_CORE_CRITICAL(!glfwInit(), "Could not initialize GLFW!!");
                }

<<<<<<< HEAD
            }

            m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
            ++s_GLFWWindowCount;

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
                event.DeleteGenericData();
            });

            glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                Event::AppEvent::WindowClosedEvent event;
                data.fn(event);
                event.DeleteGenericData();
            });

            glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
=======
>>>>>>> origin/master
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

                glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int mousecode, int action, int mods)
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

                glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double X, double Y)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        Event::MouseEvent::MouseScrooledEvent event(X, Y);
                        data.fn(event);
                    });

                glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double X, double Y)
                    {
                        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                        Event::MouseEvent::MouseMovedEvent event(X, Y);
                        data.fn(event);
                    });

            }

            void WindowsWindow::OnUpdate()
            {

                ME_PROFILE_TRACE_CALL();

                PoolInputs();
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
                    delete Input::Get();
                }
            }

            void WindowsWindow::PoolInputs()
            {
                double X, Y;
                int wx, wy;
                glfwGetWindowPos(m_Window, &wx, &wy);
                glfwGetCursorPos(m_Window, &X, &Y);
                m_Data.Input->m_MouseDelta = m_Data.Input->m_MousePos - glm::vec2{ X + wx, Y + wy };
                m_Data.Input->m_MousePos = { X + wx, Y + wy };
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

<<<<<<< HEAD
            glfwDestroyWindow(m_Window);
            --s_GLFWWindowCount;
            if(s_GLFWWindowCount == 0)
                glfwTerminate();
        }

        void WindowsWindow::SetVSync(bool enable)
        {

            ME_PROFILE_TRACE_CALL();

            if(enable)
                glfwSwapInterval(1);
            else
                glfwSwapInterval(0);

            m_Data.VSync = enable;
        }

        bool WindowsWindow::IsVSync() const
        {

            ME_PROFILE_TRACE_CALL();

            return m_Data.VSync;
=======
                return m_Data.VSync;
            }
>>>>>>> origin/master
        }
    }
}