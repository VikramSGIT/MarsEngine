#include "MarsHeader.h"
#include "Martian.h"

#include "Core/Application.h"
#include "Window/Input.h"
#include "Window/Events/WindowEvent.h"
#include "Window/Events/KeyEvent.h"
#include "Window/Events/MouseEvent.h"

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

int count = 0;

bool IsMouseInsideWindow(ImGuiWindow* window)
{
	return Window::Input::GetMouseX() > window->Pos.x && Window::Input::GetMouseY() > window->Pos.y
		&& Window::Input::GetMouseX() < window->Pos.x + window->Size.x && Window::Input::GetMouseY() < window->Pos.y + window->Size.y;
}

Martian::Martian()
	:Layer("Martian"), m_ViewportSize({ app->GetWindow().GetWidth(), app->GetWindow().GetHeight()}) {}

void Martian::OnAttach()
{
	app->GetWindow().SetVSync(true);
	renderer = ME::RenderAPI::Create(ME::RenderAPItype::ME_RENDERER_OPENGL2D);
	app->setRenderAPI(renderer);
	imgui = CreateRef<Window::imguiLayer>();
	app->GetLayerStack()->PushLayer(imgui);

	renderer->framebuffer = Framebuffer::Create({});
	background = CreateRef<ME::Rectangle>("Background", glm::vec2{ 1280 * 4, 720 * 4 }, 3);
	background->TranslateTo({ -1280.0f * 2, -720.0f * 2 });
	imgui->SetDrawData([this]() -> void
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
			m_Editor = ImGui::GetCurrentWindow();
			ImVec2 s = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != glm::vec2{ s.x, s.y })
			{
				m_ViewportSize = { s.x, s.y };
				renderer->framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

				m_Camera->SetViewport({ m_ViewportSize.x, m_ViewportSize.y });

				background->Set(m_ViewportSize * glm::vec2{ 4.0f, 4.0f });
				background->TranslateTo(-m_ViewportSize * glm::vec2{ 2.0f, 2.0f });
			}
			ImGui::Image((void*)renderer->framebuffer->getColorAttachment(), s, ImVec2{0, 1}, ImVec2{1, 0});
			ImGui::End();
			ImGui::PopStyleVar();

			ImGui::Begin("Stats");
			ImGui::Text("Up time: %ds", (int)app->GetTime());
			ImGui::Text("Frame Time: %lfms", (glfwGetTime() - app->GetTime())*1000);
			ImGui::Text("Mouse Delta: [%d %d]", (int)Window::Input::GetMouseDeltaX(), (int)Window::Input::GetMouseDeltaY());
			ImGui::Text("Vertices: %d", renderer->GetTotalVertices());
			ImGui::Text("Indices: %d", renderer->GetTotalIndices());
			ImGui::End();
		});

	shader = Shader::Create("res\\shaders\\Basic.shader");
	renderer->SetShader(shader);
	renderer->SetClearColor({ 0.4f, 0.4f, 0.4f, 1.0f });

	Player =CreateRef<ME::Rectangle>("Aadhav", glm::vec2{200.0f, 200.0f});
	renderer->AddMesh(Ref<Mesh2D>(background));
	renderer->AddMesh(Player);
	tex = Texture2D::Create("res\\textures\\android.png");

	Ref<int*> a = CreateRef<int*>(alloc<int>());

	m_Camera = CreateRef<Camera2D>(glm::vec2(m_ViewportSize.x, m_ViewportSize.y));
}

void Martian::OnUpdate(Timestep ts)
{
	if (Window::Input::IsKeyPressed(Event::Key::W))
		Player->Translate({ 0.0f, 300.0f * ts });
	if (Window::Input::IsKeyPressed(Event::Key::A))
		Player->Translate({ -300.0f * ts, 0.0f});
	if (Window::Input::IsKeyPressed(Event::Key::S))
		Player->Translate({ 0.0f, -300.0f * ts});
	if (Window::Input::IsKeyPressed(Event::Key::D))
		Player->Translate({ 300.0f * ts, 0.0f});
	if (Window::Input::IsKeyPressed(Event::Key::Space))
		Player->Rotate({90.0f * ts});
	if (Window::Input::IsKeyPressed(Event::Key::LeftShift))
		Player->Scale({ 1.005f, 1.005f});
	if (Window::Input::IsKeyPressed(Event::Key::CapsLock))
		Player->Scale({ 0.995f, 0.995f});
	if (Window::Input::IsKeyPressed(Event::Key::RightShift))
		app->GetWindow().SetVSync(!app->GetWindow().IsVSync());
	if (Window::Input::IsKeyPressed(Event::Key::Escape))
		m_Camera->Reset();
	if (Window::Input::IsMousePressed(Event::Mouse::ButtonMiddle) && Window::Input::IsMouseMoved())
	{
		m_Camera->Pan(glm::vec3(Window::Input::GetMouseDelta(), 0.0));
		background->Translate(-Window::Input::GetMouseDelta());
	}

	shader->SetUniformsMat4f("u_MVP", *m_Camera);
	tex->Bind();
}

void Martian::OnDraw() {}
void Martian::OnEvent(Event::Event& e) 
{
	Event::EventDispatcher dis(e);
	dis.Dispatch<Event::KeyEvent::KeyPressedEvent>([this](Event::Event& e) ->bool
		{
			if (Event::Key::Equal == ((Event::KeyEvent::KeyPressedEvent&)e).GetkeyCode())
			{
				renderer->AddMesh(GenQuad2D("TileMap", { 0.0 + count, 0.0}, { 50.0 + count, 0.0}, { 50.0 + count, 50.0 }, { 0.0 + count, 50.0 }, 1));
				count += 50;
				return true;
			}
			return false;
		});

	dis.Dispatch<Event::MouseEvent::MouseScrooledEvent>([this](Event::Event& e) -> bool
		{
			float offset = ((Event::MouseEvent::MouseScrooledEvent&)e).GetOffsetY();
			if(IsMouseInsideWindow(m_Editor))
				m_Camera->Zoom(offset, glm::vec2{ Window::Input::GetMouse().x - m_Editor->Pos.x, Window::Input::GetMouse().y - m_Editor->Pos.y });
			return true;
		});
}
void Martian::OnDetach() {}
