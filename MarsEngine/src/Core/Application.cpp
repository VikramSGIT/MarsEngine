#include "Application.h"
namespace ME
{
    Application* Application::s_Application = nullptr;

    Application::Application()
        :window(Window::Window::Create({ "MarsEngine", 500, 500 }))
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
        m_Projection = glm::ortho(0.0f, float(X), 0.0f, float(Y), -1.0f, 1.0f);
        renderer->SetViewPortSize(X, Y);
        return true;
    }

    void Application::METerminate()
    {
        m_LayerStack.~LayerStack();
        renderer.reset();
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

        renderer = CreateRef<ME::Renderer::OpenGL::OpenGLRendererAPI>();
        renderer->Init();
        renderer->SetClearColor({ 50.0, 50.0, 50.0, 256.0 });

#ifdef ME_IMGUI
        Ref<Window::Layer::imguiLayer> imgui = CreateRef<Window::Layer::imguiLayer>();
        m_LayerStack.PushOverlay(imgui);
#endif
        m_LayerStack.PushLayer(renderer->GetLayer());


        //This Parenthiesis is to Destroy The Stack Allocated Vertex and Index Buffers before Destroying OpenGl Context
        {
            ME::VERTEX v1[] = {
                { 100.0f, 100.0f, 1.0f, 0.0f, 0.0f}, //1
                { 200.0f, 100.0f, 1.0f, 1.0f, 0.0f}, //2
                { 200.0f, 200.0f, 1.0f, 1.0f, 1.0f}, //3
                { 100.0f, 200.0f, 1.0f, 0.0f, 1.0f}  //4
            };

            ME::VERTEX v2[] =
            {
                { 200.0f, 200.0f, 1.0f, 0.0f, 0.0f},  //1
                { 300.0f, 200.0f, 1.0f, 1.0f, 0.0f},  //2
                { 300.0f, 300.0f, 1.0f, 1.0f, 1.0f},  //3
                { 200.0f, 300.0f, 1.0f, 0.0f, 1.0f}   //4
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

            Ref<Mesh> mesh1 = CreateRef<Mesh>("Test");
            Ref<Mesh> mesh2 = CreateRef<Mesh>("Test");
            mesh1->BufferVertices(v1, sizeof(v1) / sizeof(ME::VERTEX));
            mesh1->BufferIndices(oindices1, 6);

            mesh2->BufferVertices(v2, sizeof(v2) / sizeof(ME::VERTEX));
            mesh2->BufferIndices(oindices1, 6);

            MeshQueue meshqueue;
            meshqueue.PushMesh(mesh1);
            meshqueue.PushMesh(mesh2);

            renderer->AddRenderSubmition(meshqueue, []() {});
            renderer->SetClearColor({ 0.4f, 0.4f, 0.4f, 1.0f });

            Ref<Renderer::Shader> shader = renderer->Create("res/shaders/Basic.shader");
            shader->Bind();

            Renderer::OpenGL::OpenGLTexture texture("res/textures/android.png");
            texture.Bind();
            shader->SetUniforms1i("u_Texture", texture.GetSlot());
            shader->unBind();

            double ms = 0.0;
            window->SetVSync(true);

            while (m_Running)
            {
                InstrumentorTimer timer(ms);

                //Always pass the uniforms after binding the shader

                shader->SetUniformsMat4f("u_MVP", m_Projection);

                //Updating layers
#ifdef ME_IMGUI
                imgui->SetDrawData([this, &ms]()
                    {
                        ImGui::Begin("Benchmark");
                        ImGui::Text(std::to_string(ms).substr(0, 6).c_str());
                        ImGui::SameLine();
                        ImGui::Text("ms");
                        ImGui::Text(std::to_string(1000 / (ms)).substr(0, 5).c_str());
                        ImGui::SameLine();
                        ImGui::Text("fps");
                        ImGui::End();
                    });
#endif

                for (Ref<Window::Layer::Layer> layer : m_LayerStack)
                    layer->OnUpdate();

                if (window->IsKeyPressed(Event::Key::W))
                    m_Projection = glm::translate(m_Projection, { 0.0f, 10.0f, 0.0f });

                if (window->IsKeyPressed(Event::Key::S))
                    m_Projection = glm::translate<ME_DATATYPE>(m_Projection, { 0.0f, -10.0f, 0.0f });

                if (window->IsKeyPressed(Event::Key::A))
                    m_Projection = glm::translate<ME_DATATYPE>(m_Projection, { -10.0f, 0.0f, 0.0f });

                if (window->IsKeyPressed(Event::Key::D))
                    m_Projection = glm::translate<ME_DATATYPE>(m_Projection, { 10.0f, 0.0f, 0.0f });

                if (window->IsKeyPressed(Event::Key::E))
                    m_Projection = glm::rotate<ME_DATATYPE>(m_Projection, -3.14f / 50.0f, { 0.0f, 0.0f, 1.0f });

                if (window->IsKeyPressed(Event::Key::Q))
                    m_Projection = glm::rotate<ME_DATATYPE>(m_Projection, 3.14f / 50.0f, { 0.0f, 0.0f, 1.0f });

                if (window->IsKeyPressed(Event::Key::Space))
                {
                    mesh1* glm::scale(glm::mat4(), { 1.01f, 1.01f, 1.0f });
                }
                renderer->OnUpdate();
                renderer->Draw(shader);
                window->OnUpdate();
            }
        }
        METerminate();
        glfwTerminate();
    }
}