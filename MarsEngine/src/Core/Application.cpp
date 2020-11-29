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

    oglm::Matrix4 m_MVP, m_Model;
    oglm::vec3 trans;

    oglm::Matrix4 m_proj = oglm::GenOrtho(0.0f, 300.0f, 0.0f, 300.0f, -1.0f, 1.0f);

    Camera camera;
    camera.SetProjectionView(m_proj);
    GetLogger()->SetLogging(true);

//Needed improvement : Make to handler function for this
    Ref<Renderer::RenderAPI> renderer = CreateRef<Renderer::OpenGL::OpenGLRendererAPI>();
//
//
    window->SetEventCallBack(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    renderer->Init();
    renderer->SetClearColor({256.0,256.0,0.0,256.0});

    Ref<Window::Layer::imguiLayer> imgui = CreateRef<Window::Layer::imguiLayer>();
    m_LayerStack.PushLayer(renderer->GetLayer());
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
    
    Ref<Renderer::VertexArray> va = renderer->Create();
    Ref<Renderer::VertexBuffer> vb1 = renderer->Create((void*)position, sizeof(position), GL_DYNAMIC_DRAW);

    Ref<Renderer::VertexBufferLayout> layout = renderer->CreateBufferLayout();
    layout->push(GL_FLOAT, 2);
    layout->push(GL_FLOAT, 2);
    va->AddBuffer(*vb1, *layout);

    Ref<Renderer::IndexBuffer> ib = renderer->Create(indices, 6, GL_DYNAMIC_DRAW);

    Ref<Shader> shader = renderer->Create("res/shaders/Basic.shader");
    shader->Bind();

    Renderer::OpenGL::OpenGLTexture texture("res/textures/android.png");
    texture.Bind();
    shader->SetUniforms1i("u_Texture", texture.GetSlot());

    vb1->unBind();
    va->unBind();
    ib->unBind();
    shader->unBind();

    double ms = 0.0;
    window->SetVSync(true);

    /* Loop until the user closes the window */
    while (m_Running)
    {
        InstrumentorTimer timer(ms);
        renderer->OnUpdate();
//Always pass the uniforms after binding the shader
        shader->Bind();

        shader->SetUniformsMat4f("u_MVP", m_MVP);

        renderer->Draw(*va, *ib, *shader);
//Updating layers

        imgui->SetDrawData([this, &ms]()
            {
                ImGui::Begin("Benchmark");
                ImGui::SetWindowFocus(nullptr);
                ImGui::Text("Game : ");
                ImGui::SameLine();
                ImGui::Text(std::to_string(ms).substr(0, 6).c_str());
                ImGui::SameLine();
                ImGui::Text("ms");
                ImGui::End();
            });

        for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); ++it)
            (*it)->OnUpdate();

        if (window->IsKeyPressed(Event::Key::W))
            m_Model.GetY() += 0.01;

        if (window->IsKeyPressed(Event::Key::S))
            m_Model.GetY() -= 0.01;

        if (window->IsKeyPressed(Event::Key::A))
            m_Model.GetX() -= 0.01;

        if (window->IsKeyPressed(Event::Key::D))
            m_Model.GetX() += 0.01;

        if (window->IsKeyPressed(Event::Key::E))
             m_Model = oglm::Rotate(oglm::GenIdentity(), 0, 0, -3.14/50) * m_Model;

        if (window->IsKeyPressed(Event::Key::Q))
            m_Model = oglm::Rotate(oglm::GenIdentity(), 0, 0, 3.14/50) * m_Model;

        m_MVP = camera.GetProjectionViewMat() * m_Model;

        window->OnUpdate();
    }
}
    glfwTerminate();
}