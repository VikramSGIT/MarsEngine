#ifndef MARTIAN
#define MARTIAN

#pragma once

#include "RenderAPI/Buffers.h"
#include "Window/Layers/Layer.h"
#include "RenderAPI/Renderer.h"
#include "Modules/2D/Mesh2D.h"
#include "RenderAPI/Shader.h"
#include "RenderAPI/OpenGL/OpenGL2D/OpenGLTexture2D.h"
#include "Window/Layers/ImGUI/imguiLayer.h"
#include "Modules/2D/Camera2D.h"
#include "Core/Utilites/Ref.h"

#include "imgui.h"
#include "imgui_internal.h"

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
	ME::Ref<ME::Shader> shader;
	ME::Ref<ME::Mesh2D> Player;
	ME::Ref<ME::Rectangle> background;
	ME::Ref<ME::OpenGL::OpenGLTexture2D> tex;
	std::vector<ME::Ref<ME::Mesh2D>> obj;
	ME::Ref<ME::RenderAPI> renderer;
	ME::Ref<ME::Framebuffer> framebuffer;
	ME::Ref<ME::Camera2D> m_Camera;

	glm::vec2 m_ViewportSize, m_ViewPortMousePos;
	ImGuiWindow* m_Editor;
};

#endif