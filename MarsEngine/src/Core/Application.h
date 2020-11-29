#pragma once
#include "MarsHeader.h"

#include "GL/glew.h"
#include "Vender/GLFW/glfw3.h"
#include "Logger.h"

#include <iostream>
#include <memory>
#include <functional>

#include "RenderAPI/OpenGL/OpenGLErrorHandle.h"
#include "RenderAPI/OpenGL/OpenGLRenderer.h"
#include "RenderAPI/OpenGL/OpenGLTexture.h"
#include "Window/Window.h"
#include "Window/Windows/WindowsWindow.h"
#include "Window/Events/WindowEvent.h"
#include "Window/Events/MouseEvent.h"
#include "Window/Events/KeyEvent.h"
#include "Window/Layers/LayerStack.h"
#include "Window/Layers/ImGUI/imguiLayer.h"
#include "Window/Events/KeyboardCode.h"
#include "RenderAPI\Camera.h"


class Application
{
private:
    Window::Layer::LayerStack m_LayerStack;
    bool m_Running = true;
    std::unique_ptr<Window::Window> window = std::unique_ptr<Window::Window>(Window::Window::Create({"Mars-Engine", 500, 500}));

public:
    Application();
    void Run();
    void OnEvent(Event::Event& e);
    bool OnWindowClose();

    Window::Window& GetWindow() {return *window;}
    static Application& GetInstance() { return *s_Application;}

    static Application* s_Application;
};
