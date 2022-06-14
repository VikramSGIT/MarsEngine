#ifndef ME_APPLICATION
#define ME_APPLICATION

#pragma once 

#include "Window/Layers/LayerStack.h"
#include "Window/Window.h"
#include "RenderAPI/Renderer.h"
#include "Window/Events/Event.h"
#include "Core/Utilites/Vector.h"

namespace ME
{
    class Application
    {
    public:
        Application();
        ~Application();
        void Run();
        void METerminate();

        Window::LayerStack* GetLayerStack() { return &m_LayerStack; }
        void setRenderAPI(const Ref<Renderer::RenderAPI>& api);
        Window::Window& GetWindow() { return *m_Window; }

        void UpdateNotification(Mesh2D* mesh);
        void UpdateNotification(Mesh* mesh);

        static Application& GetInstance() { return *s_Application; }
        static Application* CreateApp();
    private:
        Window::LayerStack m_LayerStack;
        Ref<Renderer::RenderAPI> m_RenderAPI;

        bool m_Running = true;
        Window::Window* m_Window;

        void OnEvent(Event::Event& e);
        bool OnWindowClose();
        bool OnWindowResize();

        double m_LastFrameTime;

        static Application* s_Application;

        friend static void Event::throwEvent(Event&);
    };

}

#endif