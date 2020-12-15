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

bool Application::OnWindowResize()
{
    unsigned int X = window->GetWidth();
    unsigned int Y = window->GetHeight();
    oglm::Matrix4<float> proj = oglm::GenOrtho<float>(0.0f, X, 0.0f, Y, -1.0f, 1.0f);
    camera->SetProjectionViewMat(proj);
    m_Projection = proj;
    renderer->SetViewPortSize(X, Y);

    return true;
}

void Application::OnEvent(Event::Event& e)
{
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
    renderer = CreateRef<Renderer::OpenGL::OpenGLRendererAPI>();
    glewInit();
    renderer->Init();
    renderer->SetClearColor({50.0, 50.0, 50.0, 256.0});

#ifdef ME_IMGUI
    Ref<Window::Layer::imguiLayer> imgui = CreateRef<Window::Layer::imguiLayer>();
    m_LayerStack.PushOverlay(imgui);
#endif
    m_LayerStack.PushLayer(renderer->GetLayer());


//This Parenthiesis is to Destroy The Stack Allocated Vertex and Index Buffers before Destroying OpenGl Context
{
    float object1[] = {
        100.0f, 100.0f, 1.0f,  //0
        200.0f, 100.0f, 1.0f,  //1
        200.0f, 200.0f, 1.0f,  //2
        100.0f, 200.0f, 1.0f   //4
    };

    float object2[] =
    {
        200.0f, 200.0f, 1.0f,  //1
        300.0f, 200.0f, 1.0f,  //2
        300.0f, 300.0f, 1.0f,  //3
        200.0f, 300.0f, 1.0f   //4
    };

    unsigned int oindices1[] =
    {
        0,1,2,
        0,3,2
    };

    unsigned int oindices2[] =
    {
        0,1,2,
        0,3,2
    };

    Renderer::Mesh mesh1("Test");
    Renderer::Mesh mesh2("Test");
    mesh1.BufferVertices(object1, sizeof(float) * 12);
    mesh1.BufferIndices(oindices1, sizeof(float)* 6);

    mesh2.BufferVertices(object2, sizeof(float)*  12);
    mesh2.BufferIndices(oindices1, sizeof(float)* 6);

    Renderer::MeshQueue meshqueue;
    meshqueue.PushMesh(mesh1);
    meshqueue.PushMesh(mesh2);

    renderer->AddRenderSubmition(meshqueue);

    Ref<Shader> shader = renderer->Create("res/shaders/Basic.shader");
    shader->Bind();

    //Renderer::OpenGL::OpenGLTexture texture("res/textures/android.png");
    //texture.Bind();
    //shader->SetUniforms1i("u_Texture", texture.GetSlot());
    shader->unBind();

    double ms = 0.0;
    window->SetVSync(true);

    /* Loop until the user closes the window */
    while (m_Running)
    {
        InstrumentorTimer timer(ms);

//Always pass the uniforms after binding the shader

        shader->SetUniformsMat4f("u_MVP", m_MVP);

        renderer->OnUpdate();
        renderer->Draw(*shader);
//Updating layers
#ifdef ME_IMGUI
        imgui->SetDrawData([this, &ms]()
            {
                ImGui::Begin("Benchmark");
                ImGui::Text(std::to_string(ms).substr(0, 6).c_str());
                ImGui::SameLine();
                ImGui::Text("ms");
                ImGui::Text(std::to_string(1000/(ms)).substr(0,5).c_str());
                ImGui::SameLine();
                ImGui::Text("fps");
                ImGui::End();
            });
#endif

        for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); ++it)
            (*it)->OnUpdate();

        if (window->IsKeyPressed(Event::Key::W))
            m_Projection = oglm::Transulate<float>(oglm::GenIdentity<float>(), 0, 10, 0) * m_Projection;

        if (window->IsKeyPressed(Event::Key::S))
            m_Projection = oglm::Transulate<float>(oglm::GenIdentity<float>(), 0, -10, 0) * m_Projection;

        if (window->IsKeyPressed(Event::Key::A))
            m_Projection = oglm::Transulate<float>(oglm::GenIdentity<float>(), -10, 0, 0) * m_Projection;

        if (window->IsKeyPressed(Event::Key::D))
            m_Projection = oglm::Transulate<float>(oglm::GenIdentity<float>(), 10, 0, 0) * m_Projection;

        if (window->IsKeyPressed(Event::Key::E))
            m_Projection = oglm::Rotate<float>(oglm::GenIdentity<float>(), 0, 0, -3.14 / 50) * m_Projection;

        if (window->IsKeyPressed(Event::Key::Q))
            m_Projection = oglm::Rotate<float>(oglm::GenIdentity<float>(), 0, 0, 3.14 / 50) * m_Projection;
        m_MVP = m_Projection * m_Model;

        for (Ref<Window::Layer::Layer> layer : m_LayerStack)
            layer->OnUpdate();

        window->OnUpdate();
    }
}
    glfwTerminate();
}