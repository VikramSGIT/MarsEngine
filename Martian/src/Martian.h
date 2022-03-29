#ifndef MARTIAN
#define MARTIAN

#include "MarsHeader.h"
#include "Core/Memory/Vector.h"

#include "Window/Window.h"
#include "Window/Input.h"
#include "RenderAPI/OpenGL/OpenGLRenderer.h"
#include "RenderAPI/OpenGL/OpenGLShader.h"
#include "Window/Layers/Layer.h"
#include "Core/Utilites/TimeStep.h"
#include "Addons/ParticleSystem.h"

class Martian : public ME::Window::Layer::Layer
{
public:
	Martian()
		:Layer("Martian"), renderer(ME::CreateRef<ME::Renderer::OpenGL::OpenGLRendererAPI>()) {}
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(ME::Timestep ts) override;
	virtual void OnDraw() override;
	virtual void OnEvent(ME::Event::Event& e) override;
private:
	
	ME::Ref<ME::Renderer::RenderAPI> renderer;
	ME::Ref<ME::Renderer::Shader> shader;
	ME::Ref<ME::Addon::ParticleSystem> particlesystem;
	ME::Ref<ME::Mesh> Player;
	std::vector<ME::Ref<ME::Mesh>> obj;
};

#endif