#ifndef MARTIAN
#define MARTIAN

#pragma once

#include "RenderAPI/Buffers.h"
#include "Window/Layers/Layer.h"
#include "RenderAPI/Renderer.h"
#include "Modules/2D/Mesh2D.h"
#include "RenderAPI/Shader.h"
#include "RenderAPI/OpenGL/OpenGLTexture.h"
#include "Window/Layers/ImGUI/imguiLayer.h"
#include "Modules/Camera.h"
#include "Core/Utilites/Ref.h"

class Martian : public ME::Window::Layer
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
	ME::Ref<ME::Window::imguiLayer> imgui;
	ME::Ref<ME::Renderer::Shader> shader;
	ME::Ref<ME::Mesh2D> Player;
	ME::Ref<ME::Rectangle> background;
	ME::Ref<ME::Renderer::OpenGL::OpenGLTexture> tex;
	std::vector<ME::Ref<ME::Mesh2D>> obj;
	ME::Ref<ME::Renderer::RenderAPI> renderer;
	ME::Ref<ME::Renderer::Framebuffer> framebuffer;
	ME::Ref<ME::Camera> m_Camera;

	glm::vec2 m_ViewportSize;
};

#endif