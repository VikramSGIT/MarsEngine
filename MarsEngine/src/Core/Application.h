#pragma once 

#include "Window/Window.h"
#include "RenderAPI/Renderer.h"
#include "Window/Events/Event.h"
#include "Core/Utilites/Vector.h"
#include "Window/Input.h"

namespace ME
{
    class Application
    {
    public:
        Application();
        ~Application();
        void Run();
        void METerminate();


        void setRenderAPI(const ref<RenderAPI>& api);

        void UpdateNotification(Mesh2D* mesh);
        void UpdateNotification(Mesh* mesh);

        double getTime() { return m_LastFrameTime; }

        Window::LayerStack& getLayerStack() { return m_LayerStack; }
        vector<Ref<Window::IOLayer>>& getIOLayerStack() { return m_IOLayerStack; }
        static Application& GetInstance() { return *s_Application; }
        Window::Window& GetWindow() { return *m_Window; }
        Window::Input& GetInput() { return *m_Input; }

        static Application* CreateApp();
    private:
        Window::LayerStack m_LayerStack;
        vector<Ref<Window::IOLayer>> m_IOLayerStack;
        Ref<RenderAPI> m_RenderAPI;

        bool m_Running = true;
        Window::Window* m_Window;
        Window::Input* m_Input;

        void OnEvent(Event::Event& e);
        bool OnWindowClose();
        bool OnWindowResize();

        double m_LastFrameTime;

        static Application* s_Application;

        friend static void Event::throwEvent(Event&);
    };

}