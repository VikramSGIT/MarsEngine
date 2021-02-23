#pragma once
#include "MarsHeader.h"

#include "GL/glew.h"
#include "Vender/GLFW/glfw3.h"
#include "Logger.h"
#include "Vender/glm/glm/glm.hpp"

#include <iostream>
#include <memory>
#include <functional>

#include "RenderAPI/OpenGL/OpenGLRenderer.h"
#include "RenderAPI/OpenGL/OpenGLTexture.h"
#include "RenderAPI/Buffers.h"

#include "RenderAPI/Mesh.h"
#include "Window/Window.h"
#include "Window/Windows/WindowsWindow.h"
#include "Window/Events/WindowEvent.h"
#include "Window/Events/MouseEvent.h"
#include "Window/Events/KeyEvent.h"
#include "Window/Layers/LayerStack.h"
#include "Window/Layers/ImGUI/imguiLayer.h"
#include "Window/Events/KeyboardCode.h"
#include "RenderAPI/Camera.h"

namespace ME
{
    class Application
    {
    private:
        Window::Layer::LayerStack m_LayerStack;
        bool m_Running = true;
        Ref<Window::Window> window;
        glm::mat4 m_Projection = glm::ortho(0.0f, 500.0f, 0.0f, 500.0f);
        Ref<Renderer::RenderAPI> renderer;
    public:
        Application();
        ~Application() = default;
        void Run();
        void OnEvent(Event::Event& e);
        bool OnWindowClose();
        bool OnWindowResize();
        void METerminate();



        Window::Window& GetWindow() { return *window; }
        static Application& GetInstance() { return *s_Application; }

        static Application* s_Application;
    };

    static Application* CreateApp();
}
