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
#include "Modules/OrthographicCameraController.h"
#include "Core/Utilites/Ref.h"
#include "Window/UI.h"

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
	ME::ref<ME::Window::imguiLayer> imgui;
	ME::ref<ME::Shader> shader;
	ME::ref<ME::Mesh2D> Player;
	ME::ref<ME::Rectangle> background;
	ME::ref<ME::OpenGL::OpenGLTexture2D> tex;
	ME::vector<ME::Ref<ME::Mesh2D>> obj;
	ME::ref<ME::RenderAPI> renderer;
	ME::ref<ME::Framebuffer> framebuffer;
	ME::ref<ME::OrthographicCameraController> m_CameraController;

	ME::Ref<ME::UIWindow> MartianUI;
	ME::Ref<ME::UIWindow> EditorUI;
	ME::Ref<ME::UIWindow> StatsUI;

	glm::vec2 m_ViewportSize, m_ViewPortMousePos;
	ImGuiWindow* m_Editor;
};

#endif