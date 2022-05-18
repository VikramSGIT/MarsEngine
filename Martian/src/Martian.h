#ifndef MARTIAN
#define MARTIAN

#pragma once

#include "RenderAPI/OpenGL/OpenGL2D/OpenGL2DRenderer.h"
#include "RenderAPI\OpenGL\OpenGLShader.h"
#include "Window/Layers/Layer.h"

#include "Modules/2D/Mesh2D.h"

class Martian : public ME::Window::Layer::Layer
{
public:
	Martian()
		:Layer("Martian"), renderer(ME::CreateRef<ME::Renderer::OpenGL::OpenGL2DRendererAPI>()) {}

	~Martian() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(ME::Timestep ts) override;
	virtual void OnDraw() override;
	virtual void OnEvent(ME::Event::Event& e) override;
private:
	
	ME::Ref<ME::Renderer::Render2DAPI> renderer;
	ME::Ref<ME::Renderer::Shader> shader;
	ME::Ref<ME::Mesh2D> Player;
	ME::Ref<ME::Renderer::OpenGL::OpenGLTexture> tex;
	std::vector<ME::Ref<ME::Mesh2D>> obj;
};

#endif