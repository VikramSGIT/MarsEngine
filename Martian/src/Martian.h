#ifndef MARTIAN
#define MARTIAN

#pragma once

#include "Window/Layers/Layer.h"
#include "RenderAPI/Renderer.h"
#include "Modules/2D/Mesh2D.h"
#include "RenderAPI/Shader.h"
#include "RenderAPI/OpenGL/OpenGLTexture.h"

class Martian : public ME::Window::Layer::Layer
{
public:
	Martian();
	~Martian() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(ME::Timestep ts) override;
	virtual void OnDraw() override;
	virtual void OnEvent(ME::Event::Event& e) override;
private:
	
	ME::Ref<ME::Renderer::Shader> shader;
	ME::Ref<ME::Mesh2D> Player;
	ME::Ref<ME::Renderer::OpenGL::OpenGLTexture> tex;
	std::vector<ME::Ref<ME::Mesh2D>> obj;
	ME::Ref<ME::Renderer::RenderAPI> renderer;
};

#endif