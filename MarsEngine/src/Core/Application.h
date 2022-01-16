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
#include "Window/Input.h"
#include "Window/Windows/WindowsWindow.h"
#include "Window/Windows/WindowsInput.h"
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

        static Application* s_Application;
    };

    extern Application* CreateApp();
}
