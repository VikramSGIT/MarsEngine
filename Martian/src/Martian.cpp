#include "MarsHeader.h"
#include "Martian.h"

#include "Core/Application.h"
#include "Window/Input.h"
#include "Window/Events/WindowEvent.h"

#include "Vender/imgui/imgui.h"
#include "Core/Memory/Scope.h"
#include "Core/Utilites/Vector.h"

ME::Application* app;

ME::Application* ME::Application::CreateApp()
{
	app = allocon<ME::Application>();

	Ref<Martian> marslayer = CreateRef<Martian>();
	app->GetLayerStack()->PushLayer(marslayer);
	return app;
}

using namespace ME;

Martian::Martian()
	:Layer("Martian"), m_ViewportSize({ app->GetWindow().GetWidth(), app->GetWindow().GetHeight()}) {}

void Martian::OnAttach()
{
	app->GetWindow().SetVSync(true);
	renderer = ME::Renderer::RenderAPI::Create(ME::Renderer::RenderAPItype::ME_RENDERER_OPENGL2D);
	app->setRenderAPI(renderer);
	imgui = CreateRef<Window::imguiLayer>();
	//app->GetLayerStack()->PushLayer(imgui);

	//renderer->framebuffer = Renderer::Framebuffer::Create({});
	background = CreateRef<ME::Rectangle>("Background", glm::vec2{ 1280, 720 }, 3);
	/*imgui->SetDrawData([this]() -> void
		{
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


			ImGui::Begin("Martian", nullptr, window_flags);
			ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					ImGui::MenuItem("New", NULL, nullptr);
					ImGui::MenuItem("Open", NULL, nullptr);
					ImGui::Separator();
					if (ImGui::MenuItem("Quit")) { Event::throwEvent(Event::AppEvent::WindowClosedEvent()); };
					ImGui::EndMenu();

				}

				ImGui::EndMenuBar();
			}
			
			ImGui::ShowDemoWindow();

            ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
			ImGui::Begin("Editor");
			ImVec2 s = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != glm::vec2{ s.x, s.y })
			{
				m_ViewportSize = { s.x, s.y };
				renderer->framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
				background->Set(m_ViewportSize);
			}
			ImGui::Image((void*)renderer->framebuffer->getColorAttachment(), s, ImVec2{0, 1}, ImVec2{1, 0});
			ImGui::End();
			ImGui::PopStyleVar();

		});*/

	shader = Renderer::Shader::Create("res\\shaders\\Basic.shader");
	renderer->SetShader(shader);
	renderer->SetClearColor({ 0.4f, 0.4f, 0.4f, 1.0f });

	Player = CreateRef<ME::Rectangle>("Aadhav", glm::vec2{200.0f, 200.0f});
	for (int i = 0; i < 600; i += 50)
		obj.emplace_back(GenQuad2D("TileMap", {0.0 + i, 0.0 + i}, {50.0 + i, 0.0 + i}, {50.0 + i, 50.0 + i}, {0.0 + i, 50.0 + i}, 1));
	
	renderer->AddMesh((Ref<Mesh2D>(background)));
	renderer->AddMesh(obj);
	renderer->AddMesh(Player);
	tex = CreateRef<Renderer::OpenGL::OpenGLTexture>("res\\textures\\android.png");
	tex->Bind();
	shader->SetUniforms1i("u_Texture", tex->GetSlot());

	m_Camera = CreateRef<Camera>(glm::ortho(0.0f, m_ViewportSize.x, 0.0f, m_ViewportSize.y, -1.0f, 1.0f));
}

void Martian::OnUpdate(Timestep ts)
{
	if (Window::Input::IsKeyPressed(Event::Key::W))
		Player->Translate({ 0.0f, 300.0f * ts});
	if (Window::Input::IsKeyPressed(Event::Key::A))
		Player->Translate({ -300.0f * ts, 0.0f});
	if (Window::Input::IsKeyPressed(Event::Key::S))
		Player->Translate({ 0.0f, -300.0f * ts});
	if (Window::Input::IsKeyPressed(Event::Key::D))
		Player->Translate({ 300.0f * ts, 0.0f});
	if (Window::Input::IsKeyPressed(Event::Key::Space))
		Player->Rotate(90.0f * ts);
	if (Window::Input::IsKeyPressed(Event::Key::LeftShift))
		Player->Scale({ 1.005f, 1.005f});
	if (Window::Input::IsKeyPressed(Event::Key::CapsLock))
		Player->Scale({ 0.995f, 0.995f});
	if (Window::Input::IsKeyPressed(Event::Key::RightShift))
		app->GetWindow().SetVSync(!app->GetWindow().IsVSync());
	if (Window::Input::IsKeyPressed(Event::Key::Escape))
		Player->Reset();

	m_Camera->SetProjectionViewMat(glm::ortho(0.0f, m_ViewportSize.x, 0.0f, m_ViewportSize.y, -1.0f, 1.0f));
	shader->SetUniformsMat4f("u_MVP", *m_Camera);
}

void Martian::OnDraw() {}
void Martian::OnEvent(Event::Event& e) {}
void Martian::OnDetach() {}
