#include "Application.h"

Application* Application::s_Application = nullptr;

Application::Application()
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

void Application::OnEvent(Event::Event& e)
{
    Event::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Event::AppEvent::WindowClosedEvent>(std::bind(&Application::OnWindowClose, this));

    if(e.GetEventType() == Event::EventType::KeyPressed)
    {
        
    }

    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
    {
        if(e.Handled)
            break;
        (*it)->OnEvent(e);
    }
}

void Application::Run()
{

    ME_PROFILE_TRACE_CALL();

    logger->SetLogging(true);
    Renderer::OpenGL::OpenGLRendererAPI renderer;
    window->SetEventCallBack(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    renderer.Init();
    renderer.SetClearColor({256.0,256.0,0.0,256.0});

    Window::Layer::imguiLayer* imgui = new Window::Layer::imguiLayer();
    m_LayerStack.PushLayer(renderer.GetLayer());
    m_LayerStack.PushOverlay(imgui);

//This Parenthiesis is to Destroy The Stack Allocated Vertex and Index Buffers before Destroying OpenGl Context
{
    float position[] = {
        100.0f, 100.0f, 0.0f, 0.0f, //0
        200.0f, 100.0f, 1.0f, 0.0f, //1
        200.0f, 200.0f, 1.0f, 1.0f, //2
        100.0f, 200.0f, 0.0f, 1.0f  //3
    };

    unsigned int indices[] =
    {
        0,1,2,
        0,3,2
    };
    
    Ref<Renderer::VertexArray> va = renderer.Create();
    Ref<Renderer::VertexBuffer> vb1 = renderer.Create((void*)position, sizeof(position), GL_DYNAMIC_DRAW);

    Ref<Renderer::VertexBufferLayout> layout = renderer.CreateBufferLayout();
    layout->push(GL_FLOAT, 2);
    layout->push(GL_FLOAT, 2);
    va->AddBuffer(*vb1, *layout);

    Ref<Renderer::IndexBuffer> ib = renderer.Create(indices, 6, GL_DYNAMIC_DRAW);

    Renderer::OpenGL::OpenGLShader shader("res/shaders/Basic.shader");
    shader.Bind();

    Renderer::OpenGL::OpenGLTexture texture("res/textures/android.png");
    texture.Bind();
    shader.SetUniforms1i("u_Texture", texture.GetSlot());

    vb1->unBind();
    va->unBind();
    ib->unBind();
    shader.unBind();
    
    float x = 0.0;

    /* Loop until the user closes the window */
    while (m_Running)
    {
        renderer.OnUpdate();
//Always pass the uniforms after binding the shader
        shader.Bind();

        shader.SetUniformsMat4f("u_MVP", m_result);

        renderer.Draw(*va, *ib, shader);
//Updating layers
        for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); ++it)
            (*it)->OnUpdate();

        if (window->IsKeyPressed(Event::Key::W))
            oglm::TransY(m_trans, trans.y++);

        if (window->IsKeyPressed(Event::Key::S))
            oglm::TransY(m_trans, trans.y--);

        if (window->IsKeyPressed(Event::Key::A))
            oglm::TransX(m_trans, trans.x--);

        if (window->IsKeyPressed(Event::Key::D))
            oglm::TransX(m_trans, trans.x++);

        m_result = m_trans * m_proj;

        window->OnUpdate();

    }
}
    ME_PROFILE_PRINTSTACK();
    glfwTerminate();
}