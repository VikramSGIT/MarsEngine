#include "Martian.h"
#include "Core/Application.h"
#include "Utils/Timer.h"

ME::Application* app;
extern ME::Application* ME::CreateApp()
{
	app = new Application;

	Ref<Martian> marslayer = CreateRef<Martian>();
	app->GetLayerStack()->PushLayer(marslayer);
	return app;
}

void Martian::OnAttach()
{
	renderer->Init();
	shader = ME::CreateRef<ME::Renderer::OpenGL::OpenGLShader>("D:\\Projects\\MarsEngine_commit\\Martian\\res\\shaders\\Basic.shader");
	renderer->SetShader(shader);
	renderer->SetClearColor({ 0.4f, 0.4f, 0.4f, 1.0f });

	//queue.SetAllocationMode(ME::ALLOCMODE::DISTRIBUTED);
	queue.PushMeshes({ ME::GenRect("Test", { 50.0f, 50.0f }), ME::GenRect("Test", { 50.0f, 50.0f }), ME::GenRect("Test", { 50.0f, 50.0f }) });
	queue.PushMesh(ME::GenRect("Test", { 50.0f, 50.0f }));

	ME::Addon::ParticleProps props;
	props.parent = ME::GenRect("Particles System", { 30.0f, 30.0f }, 1.0f);
	props.count = 10u;
	props.transform = glm::translate(glm::scale(glm::mat4(), { 0.99f, 0.99f, 1.0f }), { -2.0f, 0.0f, 0.0f });
	props.lifetime = ME::Utils::Time::seconds(10ll);

	particlesystem = ME::CreateRef<ME::Addon::ParticleSystem>(props);
	queue.PushAddon(*particlesystem);
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
	{
		particlesystem->Emit({ 640.0f, 360.0f, 1.0f });
		queue.GetMeshes().at(0)->Translate({ 2.0f, 2.0f, 0.0f });
		queue.GetMeshes().at(1)->Translate({ 1.0f, 1.0f, 0.0f });
		queue.GetMeshes().at(2)->Translate({ 0.0f, 2.0f, 0.0f });
		queue.GetMeshes().at(3)->Translate({ 2.0f, 0.0f, 0.0f });
	}
	particlesystem->OnUpdate();
	renderer->OnUpdate();
}

void Martian::OnDraw()
{
	renderer->OnDraw();
}

void Martian::OnEvent(ME::Event::Event& e)
{
	
}
