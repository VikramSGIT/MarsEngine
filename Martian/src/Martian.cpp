#include "MarsHeader.h"
#include "Martian.h"

#include "Core/Application.h"
#include "Window/Input.h"
#include "Window/Events/WindowEvent.h"
#include "Window/Events/KeyEvent.h"
#include "Window/Events/MouseEvent.h"
#include "Window/ImGui/ImGuiUI.h"

#include "CUDAInterface/kernel.h"

#include "Vender/imgui/imgui.h"
#include "Vender/MTL/Scope.h"
#include "Vender/MTL/Vector.h"
#include "glm/glm/gtc/type_ptr.hpp"

ME::Application* app;

ME::Application* ME::Application::CreateApp()
{
	app = allocon<ME::Application>();

	Ref<Martian> marslayer = CreateRef<Martian>();
	app->getLayerStack().PushLayer(marslayer);
	return app;
}

void PositionCallback(ME::Ref<ME::Mesh2D>& mesh, const glm::vec2& position) {
	mesh->Translate(position);
}

void ScaleCallback(ME::Ref<ME::Mesh2D>& mesh, const glm::vec2& scale) {
	mesh->Scale(scale);
}

void RotateCallback(ME::Ref<ME::Mesh2D>& mesh, const ME_DATATYPE& rotate) {
	mesh->Rotate(rotate);
}


using namespace ME;

bool IsMouseInsideWindow(ImGuiWindow* window)
{
	return Window::Input::GetMouseX() > window->Pos.x && Window::Input::GetMouseY() > window->Pos.y
		&& Window::Input::GetMouseX() < window->Pos.x + window->Size.x && Window::Input::GetMouseY() < window->Pos.y + window->Size.y;
}

Martian::Martian()
	:Layer("Martian"), m_ViewportSize(app->GetWindow().getWindowSize()) {}

void Martian::OnAttach()
{
	app->GetWindow().setWindowVSync(true);
	renderer = ME::RenderAPI::Create(ME::RenderAPItype::ME_RENDERER_OPENGL2D);
	app->setRenderAPI(renderer);

	m_CameraController = CreateRef<OrthographicCameraController>(glm::vec2{ 1280, 720 });

	MartianUI = CreateRef<ImGuiUIWindow>();
	EditorUI = CreateRef<ImGuiUIWindow>();
	StatsUI = CreateRef<ImGuiUIWindow>();
	Properties = CreateRef<ImGuiUIWindow>();

	MartianUI->setUIWindowTitle("Matrian");
	EditorUI->setUIWindowTitle("Editor");
	StatsUI->setUIWindowTitle("Stats");
	Properties->setUIWindowTitle("Properties");

	MartianUI->setCallbackFunction(Event::throwEvent);
	EditorUI->setCallbackFunction(Event::throwEvent);
	StatsUI->setCallbackFunction(Event::throwEvent);
	Properties->setCallbackFunction(Event::throwEvent);

	app->getIOLayerStack().push_back(MartianUI);
	app->getIOLayerStack().push_back(EditorUI);
	app->getIOLayerStack().push_back(StatsUI);
	app->getIOLayerStack().push_back(Properties);

	imgui = CreateRef<Window::imguiLayer>();
	app->getLayerStack().PushLayer(imgui);

	renderer->framebuffer = Framebuffer::Create({});
	background = genRect("Background", glm::vec2{ 1280 * 4, 720 * 4 }, 3);
	background->Translate({ -1280.0f * 2, -720.0f * 2 });

	shader = Shader::Create("res\\shaders\\Basic.shader");
	renderer->SetShader(shader);
	renderer->SetClearColor({ 0.4f, 0.4f, 0.4f, 1.0f });

	Player = genRect("Aadhav", glm::vec2{ 200.0f, 200.0f });
	renderer->AddMesh(background);
	renderer->AddMesh(Player);
	tex = Texture2D::Create("res\\textures\\android.png");

	m_Meshes.push_back(Player);
	m_Names.push_back(Player->getName().c_str());

	m_CameraController = CreateRef<OrthographicCameraController>(m_ViewportSize);
	app->getLayerStack().PushLayer(m_CameraController);

	currMeshID = 0;

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


			MartianUI->UIWindowBegin(dockspace_flags);
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
					if (ImGui::MenuItem("Quit")) { Event::throwEvent(Event::WindowEvent::WindowCloseEvent(&app->GetWindow())); };
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
			
			ImGui::ShowDemoWindow();

			MartianUI->UIWindowEnd();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

			EditorUI->UIWindowBegin(0);
			m_Editor = ImGui::GetCurrentWindow();
			ImVec2 s = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != glm::vec2{ s.x, s.y })
			{
				m_ViewportSize = { s.x, s.y };
				renderer->framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

				background->Scale(m_ViewportSize * glm::vec2{ 4.0f, 4.0f });
				background->Translate(-m_ViewportSize * glm::vec2{ 2.0f, 2.0f });
			}
			ImGui::Image((void*)renderer->framebuffer->getColorAttachment(), s, ImVec2{0, 1}, ImVec2{1, 0});
			EditorUI->UIWindowEnd();

			ImGui::PopStyleVar();

			StatsUI->UIWindowBegin(0);
			ImGui::Text("Up time: %ds", (int)app->getTime());
			ImGui::Text("Frame Time: %lf ms", (glfwGetTime() - app->getTime())*1000);
			ImGui::Text("Mouse Delta: [%d %d]", (int)Window::Input::GetMouseDeltaX(), (int)Window::Input::GetMouseDeltaY());
			ImGui::Text("Vertices: %d", renderer->GetTotalVertices());
			ImGui::Text("Indices: %d", renderer->GetTotalIndices());
			ImGui::Text("Editor Size: %dx%d", EditorUI->getUIWindowSize().x, EditorUI->getUIWindowSize().y);
			ImGui::Text("My Size: %dx%d", StatsUI->getUIWindowSize().x, StatsUI->getUIWindowSize().y);
			ImGui::Text("Title: %s", StatsUI->getUIWindowTitle().c_str());
			ImGui::Text("Focus: %d", StatsUI->isUIWindowFocused());
			ImGui::Text("Visibility: %d", StatsUI->isUIWindowVisibile());

			ImGui::ListBox("Meshes", &currMeshID, m_Names.begin(), m_Meshes.size());
			Ref<Mesh2D> currMesh = m_Meshes[currMeshID];
			StatsUI->UIWindowEnd();
			

			Properties->UIWindowBegin(0);

			ImGui::Text("Mesh Properties");
			ImGui::PushItemWidth(200);

			ImGui::BeginGroup();
			ImGui::DragFloat2("Position", glm::value_ptr(currMesh->getMeshData()->translation), 1.0f, -500.0f, 500.0f);
			if (ImGui::IsItemEdited()) { PositionCallback(Player, currMesh->getMeshData()->translation); } ImGui::SameLine();
			ImGui::EndGroup();

			ImGui::BeginGroup();
			ImGui::DragFloat2("Scale", glm::value_ptr(currMesh->getMeshData()->scale), 0.01f, 0.01f, 2.0f);
			if (ImGui::IsItemEdited()) { ScaleCallback(Player, currMesh->getMeshData()->scale); } ImGui::SameLine();
			ImGui::EndGroup();

			ImGui::BeginGroup();
			ImGui::DragFloat("Rotation", &currMesh->getMeshData()->rotation, 1.0f, 0.0f, 360.0f);
			if (ImGui::IsItemEdited()) { RotateCallback(Player, currMesh->getMeshData()->rotation); }
			ImGui::EndGroup();

			ImGui::PopItemWidth();
			Properties->UIWindowEnd();
		});
}	

void Martian::OnUpdate(Timestep ts)
{
	shader->SetUniformsMat4f("u_MVP", m_CameraController->getCamera());
	tex->Bind();
}

void Martian::OnDraw() {}
void Martian::OnEvent(Event::Event& e) {}
void Martian::OnDetach() {}
