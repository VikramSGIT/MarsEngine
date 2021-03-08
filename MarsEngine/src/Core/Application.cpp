#include "Application.h"
namespace ME
{
    Application* Application::s_Application = nullptr;

    Application::Application()
        :window(Window::Window::Create({ "MarsEngine" }))
    {

        ME_PROFILE_TRACE_CALL();

        s_Application = this;
    }

    bool Application::OnWindowClose()
    {
        ME_PROFILE_TRACE_CALL();

        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize()
    {

        ME_PROFILE_TRACE_CALL();
        return true;
    }

    void Application::METerminate()
    {

        ME_PROFILE_TRACE_CALL();

        m_LayerStack.~LayerStack();
    }

    void Application::OnEvent(Event::Event& e)
    {

        ME_PROFILE_TRACE_CALL();

        Event::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Event::AppEvent::WindowClosedEvent>(std::bind(&Application::OnWindowClose, this));
        dispatcher.Dispatch<Event::AppEvent::WindowResizeEvent>(std::bind(&Application::OnWindowResize, this));

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            if (e.Handled)
                break;

            (*it)->OnEvent(e);
        }
    }

    void Application::Run()
    {

        ME_PROFILE_TRACE_CALL();

        GetLogger()->SetLogging(true);
        window->SetEventCallBack(std::bind(&Application::OnEvent, this, std::placeholders::_1));

        while (m_Running)
        {
            for (Ref<Window::Layer::Layer> layer : m_LayerStack)
            {
                layer->OnUpdate();
                layer->OnDraw();
            }

            window->OnUpdate();
        }
        METerminate();
    }
}