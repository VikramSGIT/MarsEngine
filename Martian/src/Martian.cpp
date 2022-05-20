#include "MarsHeader.h"
#include "Martian.h"

#include "Modules/Mesh.h"
#include "Core/Application.h"
#include "Core/Utilites/TimeStep.h"
#include "Window/Input.h"

ME::Application* app;

extern ME::Application* ME::CreateApp()
{
	app = allocon<ME::Application>();

	Ref<Martian> marslayer = CreateRef<Martian>();
	app->GetLayerStack()->PushLayer(marslayer);
	return app;
}

using namespace ME;

void Martian::OnAttach()
{
	app->GetWindow().SetVSync(true);

	renderer->Init();
	shader = CreateRef<Renderer::OpenGL::OpenGLShader>("res\\shaders\\Basic.shader");
	renderer->SetShader(shader);
	renderer->SetClearColor({ 0.4f, 0.4f, 0.4f, 1.0f });

	Player = GenRect2D("Aadhav", {200.0f, 200.0f});  
	for (int i = 0; i < 600; i += 50)
		obj.emplace_back(GenQuad2D("TileMap", {0.0 + i, 0.0 + i}, {50.0 + i, 0.0 + i}, {50.0 + i, 50.0 + i}, {0.0 + i, 50.0 + i}, 1));
	
	Ref<MeshQueue2D> queue1 = CreateRef<MeshQueue2D>(), queue2 = CreateRef<MeshQueue2D>();
	queue1->PushMesh(Player);
	queue2->PushMeshes(obj);
	tex = CreateRef<Renderer::OpenGL::OpenGLTexture>("res\\textures\\android.png");
	renderer->AddRenderSubmition(queue1, [this]()
		{
			tex->Bind();
			shader->SetUniforms1i("u_Texture", tex->GetSlot());
		});

	Mesh2D* ms = (Mesh2D*)Player.ptr();
	std::cout << ms->isValid() << std::endl;

	renderer->AddRenderSubmition(queue2, []() {});
	glm::mat4 ortho = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
	shader->SetUniformsMat4f("u_MVP", ortho);
}

void Martian::OnDetach()
{
	renderer.reset();
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

	renderer->OnUpdate();
}

void Martian::OnDraw()
{
	renderer->OnDraw();
}

void Martian::OnEvent(Event::Event& e)
{

}
