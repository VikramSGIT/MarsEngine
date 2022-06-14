#include "MarsHeader.h"

#include "Application.h"
#include "Window/Events/WindowEvent.h"
#include "Window/Windows/WindowsWindow.h"
#include "Core/Utilites/Ref.h"
#include "Logger.h"
#include "Vender/glm/glm/glm.hpp"
#include "Utilites/TimeStep.h"

#include <functional>
namespace ME
{
    Application* Application::s_Application = nullptr;

    Application::Application()
        :m_Window(Window::Window::Create({ "MarsEngine" })), m_LastFrameTime(0)
    {
        ME_PROFILE_TRACE_CALL();

        s_Application = this;
    }

    Application::~Application()
    {
        delete m_Window;
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

        m_LayerStack.clear();
        m_RenderAPI.reset();
    }

    void Application::setRenderAPI(const Ref<Renderer::RenderAPI>& api)
    {
        m_RenderAPI = api;
        m_LayerStack.PushLayer(api);
    }

    void Application::OnEvent(Event::Event& e)
    {

        ME_PROFILE_TRACE_CALL();

        Event::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Event::AppEvent::WindowClosedEvent>(std::bind(&Application::OnWindowClose, this));
        dispatcher.Dispatch<Event::AppEvent::WindowResizeEvent>(std::bind(&Application::OnWindowResize, this));

        for (auto it : m_LayerStack)
        {
            if (e.Handled)
                break;

            it->OnEvent(e);
        }
    }
    void Application::UpdateNotification(Mesh* mesh)
    {
        m_RenderAPI->PushUpdate(mesh);
    }

    void Application::UpdateNotification(Mesh2D* mesh)
    {
        m_RenderAPI->PushUpdate(mesh);
    }
    void Application::Run()
    {

        ME_PROFILE_TRACE_CALL();

        m_Window->SetEventCallBack(std::bind(&Application::OnEvent, this, std::placeholders::_1));

        while (m_Running)
        {
            double curtime = glfwGetTime(); // TODO: Make it glfw independent
            Timestep ts = curtime - m_LastFrameTime;
            m_LastFrameTime = curtime;

            for (Ref<Window::Layer> layer : m_LayerStack)
                layer->OnUpdate(ts);

            for (Ref<Window::Layer> layer : m_LayerStack)
                layer->OnDraw();

            m_Window->OnUpdate();
        }
        METerminate();
    }
}