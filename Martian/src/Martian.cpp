#include "Martian.h"
#include "Core/Application.h"
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

	queue.SetAllocationMode(ME::ALLOCMODE::DISTRIBUTED);
	queue.PushMesh(ME::GenRect("Test", { 50.0f, 50.0f }));
	queue.PushMesh(ME::GenRect("Test", { 50.0f, 50.0f }, {100.0f, 100.0f}));

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
		queue.GetMeshes().at(0)->Translate({ 2.0f, 2.0f, 0.0f });
	}
	renderer->OnUpdate();
}

void Martian::OnDraw()
{
	renderer->OnDraw();
}

void Martian::OnEvent(ME::Event::Event& e)
{
	
}
