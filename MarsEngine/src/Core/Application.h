#ifndef ME_APPLICATION
#define ME_APPLICATION

#include "MarsHeader.h"

#include "Logger.h"
#include "Vender/glm/glm/glm.hpp"
#include "Utilites/TimeStep.h"
#include "Window/Layers/LayerStack.h"
#include "Window/Window.h"

#include <iostream>
#include <memory>
#include <functional>


namespace ME
{
    class Application
    {
    public:
        Application();
        ~Application();
        void Run();
        void METerminate();

        Window::Layer::LayerStack* GetLayerStack() { return &m_LayerStack; }
        Window::Window& GetWindow() { return *m_Window; }
        static Application& GetInstance() { return *s_Application; }

    private:
        Window::Layer::LayerStack m_LayerStack;
        bool m_Running = true;
        Window::Window* m_Window;

        void OnEvent(Event::Event& e);
        bool OnWindowClose();
        bool OnWindowResize();

        float m_LastFrameTime;

        static Application* s_Application;
    };

    extern Application* CreateApp();
}

#endif