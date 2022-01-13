#include "Martian.h"
#include "Core/Application.h"
#include "Utilites/Timer.h"
#include "Core/Memory/SafePointer.h"

ME::Application* app;
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

	Player = ME::GenRect("Aadhav", { 100.0f, 100.0f });
	std::vector<ME::Ref<ME::Mesh>> obj = { ME::GenRect("Aadhav", { 100.0f, 100.0f }), ME::GenRect("Aadhav", { 100.0f, 100.0f }), ME::GenRect("Aadhav", { 100.0f, 100.0f }) };
	
	queue->PushMesh(Player);
	queue->PushMeshes(obj);

	/*ME::Addon::ParticleProps props;
	props.parent = ME::GenRect("Particles System", { 30.0f, 30.0f }, 1);
	props.count = 10u;
	props.transform = glm::translate(glm::mat4(), { 0.1f, 0.0f, 0.0f });
	props.lifetime = ME::Utils::Time::seconds(10ll);

	particlesystem = ME::CreateRef<ME::Addon::ParticleSystem>(props);
	queue->PushAddon(*particlesystem);*/

	renderer->AddRenderSubmition(queue, []() {});
	glm::mat4 ortho = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
	shader->SetUniformsMat4f("u_MVP", ortho);
}

void Martian::OnDetach()
{
	renderer.reset();
}

void Martian::OnUpdate()
{
	if (app->GetWindow().IsKeyPressed(ME::Event::Key::Space))
		particlesystem->Emit({100.0, 100.0, 0.0});
	if (app->GetWindow().IsKeyPressed(ME::Event::Key::Up))
		Player->Translate({ 0.0f, 3.0f, 0.0f });
	if (app->GetWindow().IsKeyPressed(ME::Event::Key::Left))
		Player->Translate({ -3.0f, 0.0, 0.0f });
	if (app->GetWindow().IsKeyPressed(ME::Event::Key::Down))
		Player->Translate({ 0.0f, -3.0f, 0.0f });
	if (app->GetWindow().IsKeyPressed(ME::Event::Key::Right))
		Player->Translate({ 3.0f, 0.0f, 0.0f });

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
