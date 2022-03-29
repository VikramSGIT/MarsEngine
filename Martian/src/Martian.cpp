#include "Martian.h"
#include "Core/Application.h"
#include "Utilites/Timer.h"
#include "Core/Memory/SafePointer.h"


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
	app->GetWindow().SetVSync(false);

	renderer->Init();
	shader = CreateRef<Renderer::OpenGL::OpenGLShader>("res\\shaders\\Basic.shader");
	renderer->SetShader(shader);
	renderer->SetClearColor({ 0.4f, 0.4f, 0.4f, 1.0f });

	Player = GenRect("Aadhav", { 200.0f, 200.0f });
	for (int i = 0; i < 600; i += 50)
		obj.emplace_back(GenQuad("TileMap", {0.0 + i, 0.0 + i}, {50.0 + i, 0.0 + i}, {50.0 + i, 50.0 + i}, {0.0 + i, 50.0 + i}, 1));
	
	Ref<MeshQueue> queue1 = CreateRef<MeshQueue>(), queue2 = CreateRef<MeshQueue>();
	queue1->PushMesh(Player);
	queue2->PushMeshes(obj);

	/*Addon::ParticleProps props;
	props.parent = GenRect("Particles System", { 30.0f, 30.0f }, 1);
	props.count = 10u;
	props.transform = glm::translate(glm::mat4(), { 0.1f, 0.0f, 0.0f });
	props.lifetime = Utils::Tiseconds(10ll);

	particlesystem = CreateRef<Addon::ParticleSystem>(props);
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

void Martian::OnUpdate(Timestep ts)
{
	if (Window::Input::IsKeyPressed(Event::Key::W))
		Player->Translate({ 0.0, 300.0 * ts, 0.0 });
	if (Window::Input::IsKeyPressed(Event::Key::A))
		Player->Translate({ -300.0 * ts, 0.0, 0.0 });
	if (Window::Input::IsKeyPressed(Event::Key::S))
		Player->Translate({ 0.0, -300.0 * ts, 0.0 });
	if (Window::Input::IsKeyPressed(Event::Key::D))
		Player->Translate({ 300.0 * ts, 0.0, 0.0 });
	if (Window::Input::IsKeyPressed(Event::Key::Space))
		app->GetWindow().SetVSync(!app->GetWindow().IsVSync());
	//particlesystem->OnUpdate();
	renderer->OnUpdate();
}

void Martian::OnDraw()
{
	renderer->OnDraw();
}

void Martian::OnEvent(Event::Event& e)
{

}
