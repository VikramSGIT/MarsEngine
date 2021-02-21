#include "WindowsWindow.h"
#include "Vender\imgui\imgui.h"

namespace Window
{
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
            m_Data.keystack = m_keystack;

            if (s_GLFWWindowCount == 0)
            {
                if(!glfwInit())
                {
                    ME_CORE_CRITICAL("Could not initialize GLFW!!");
                }

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
                {
                    case GLFW_PRESS:
                    {
#ifdef ME_IMGUI
                        ImGuiIO& io = ImGui::GetIO();
                        if (!io.WantCaptureKeyboard)
#endif
                        {
                            auto it = std::find(data.keystack->begin(), data.keystack->end(), key);
                            if (it == data.keystack->end())
                            {
                                data.keystack->push_back(key);
                            }
                        }
                        Event::KeyEvent::KeyPressedEvent event(key, 0);
                        keyrepeatcount = 0;
                        data.fn(event);
                        event.DeleteGenericData();
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        auto it = std::find(data.keystack->begin(), data.keystack->end(), key);
                        if (it != data.keystack->end())
                            data.keystack->erase(it);
                        Event::KeyEvent::KeyReleasedEvent event(key);
                        data.fn(event);
                        event.DeleteGenericData();
                        break;
                    }
                    case GLFW_REPEAT:
                    {
                        Event::KeyEvent::KeyPressedEvent event(key, keyrepeatcount);
                        keyrepeatcount++;
                        data.fn(event);
                        event.DeleteGenericData();
                        break;
                    }
                }
            });

            glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                    case GLFW_PRESS:
                    {
#ifdef ME_IMGUI
                        ImGuiIO& io = ImGui::GetIO();
                        if (!io.WantCaptureKeyboard)
#endif
                        {
                            auto it = std::find(data.keystack->begin(), data.keystack->end(), button);
                            if (it == data.keystack->end())
                                data.keystack->push_back(button);
                        }
                        Event::Mouse::MouseButtonPressedEvent event(button);
                        data.fn(event);
                        event.DeleteGenericData();
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        auto it = std::find(data.keystack->begin(), data.keystack->end(), button);
                        if (it != data.keystack->end())
                            data.keystack->erase(it);
                        Event::Mouse::MouseButtonReleasedEvent event(button);
                        data.fn(event);
                        event.DeleteGenericData();
                        break;
                    }
                }
            });

            glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double X, double Y)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                Event::Mouse::MouseScrooledEvent event(X,Y);
                event.DeleteGenericData();
                data.fn(event);
            });

            glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double X, double Y)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                Event::Mouse::MouseMovedEvent event(X,Y);
                event.DeleteGenericData();
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
        }
    }
}