#include "Martian.h"
#include "Core/Application.h"
#include "Utilites/Timer.h"
#include "Core/Memory/SafePointer.h"

ME::Application* app;

std::vector<ME::Ref<ME::Mesh>> obj;

extern ME::Application* ME::CreateApp()
{
	app = allocon<Application>();

	Ref<Martian> marslayer = CreateRef<Martian>();
	app->GetLayerStack()->PushLayer(marslayer);
	return app;
}

void Martian::OnAttach()
{
	//app->GetWindow().SetVSync(false);

	renderer->Init();
	shader = ME::CreateRef<ME::Renderer::OpenGL::OpenGLShader>("C:\\Projects\\MarsEngine\\Martian\\res\\shaders\\Basic.shader");
	renderer->SetShader(shader);
	renderer->SetClearColor({ 0.4f, 0.4f, 0.4f, 1.0f });

	Player = ME::GenRect("Aadhav", { 200.0f, 200.0f });
	for (int i = 0; i < 200; i += 50)
	{
		obj.emplace_back(ME::GenQuad("TileMap", {0.0 + i, 0.0 + i}, {50.0 + i, 0.0 + i}, {50.0 + i, 50.0 + i}, {0.0 + i, 50.0 + i}, 1));
	}
	
	ME::Ref<ME::MeshQueue> queue1 = ME::CreateRef<ME::MeshQueue>(), queue2 = ME::CreateRef<ME::MeshQueue>();
	queue1->PushMesh(Player);
	queue2->PushMeshes(obj);

	/*ME::Addon::ParticleProps props;
	props.parent = ME::GenRect("Particles System", { 30.0f, 30.0f }, 1);
	props.count = 10u;
	props.transform = glm::translate(glm::mat4(), { 0.1f, 0.0f, 0.0f });
	props.lifetime = ME::Utils::Time::seconds(10ll);

	particlesystem = ME::CreateRef<ME::Addon::ParticleSystem>(props);
	queue->PushAddon(*particlesystem);*/

	renderer->AddRenderSubmition(queue1, []() {});
	renderer->AddRenderSubmition(queue2, []() {});
	glm::mat4 ortho = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
	shader->SetUniformsMat4f("u_MVP", ortho);
}

void Martian::OnDetach()
{
	renderer.reset();
}

void Martian::OnUpdate(ME::Timestep ts)
{
	if (ME::Window::Input::IsKeyPressed(ME::Event::Key::W))
		Player->Translate({ 0.0, 300.0 * ts, 0.0 });
	if (ME::Window::Input::IsKeyPressed(ME::Event::Key::A))
		Player->Translate({ -300.0 * ts, 0.0, 0.0 });
	if (ME::Window::Input::IsKeyPressed(ME::Event::Key::S))
		Player->Translate({ 0.0, -300.0 * ts, 0.0 });
	if (ME::Window::Input::IsKeyPressed(ME::Event::Key::D))
		Player->Translate({ 300.0 * ts, 0.0, 0.0 });
	if (ME::Window::Input::IsKeyPressed(ME::Event::Key::Space))
		app->GetWindow().SetVSync(!app->GetWindow().IsVSync());
	//particlesystem->OnUpdate();
	renderer->OnUpdate();
}

void Martian::OnDraw()
{
	renderer->OnDraw();
}

void Martian::OnEvent(ME::Event::Event& e)
{
	
}
