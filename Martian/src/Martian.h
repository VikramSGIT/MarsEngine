#pragma once

#include "MarsHeader.h"
#include "RenderAPI/OpenGL/OpenGLRenderer.h"
#include "RenderAPI/OpenGL/OpenGLShader.h"
#include "RenderAPI/OpenGL/OpenGLTexture.h"
#include "Window/Layers/Layer.h"

class Martian : public ME::Window::Layer::Layer
{
public:
	Martian()
		:Layer("Martian") {}
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnDraw() override;
	virtual void OnEvent(ME::Event::Event& e) override;
private:
	ME::Ref<ME::Renderer::RenderAPI> renderer = ME::CreateRef<ME::Renderer::OpenGL::OpenGLRendererAPI>();
	ME::Ref<ME::Renderer::Shader> shader;
	ME::MeshQueue queue;
};