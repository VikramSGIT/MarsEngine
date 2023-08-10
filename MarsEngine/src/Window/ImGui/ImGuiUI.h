#pragma once

#include "MarsFlags.h"

#include "Window/UI.h"
#include "Window/Layers/Layer.h"

#include "Vender/glm/glm/glm.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <functional>
namespace ME
{
	class ImGuiUIWindow : public UIWindow
	{
	public:
		ImGuiUIWindow() = default;
		virtual void OnIOUpdate() override;

		virtual bool UIWindowBegin(const int& flags = 0) override;
		virtual void UIWindowEnd() override;

		virtual void setUIWindowPosition(const glm::uvec2& pos) override { m_NativeUIWindow->Pos = ImVec2(pos.x, pos.y); }
		virtual void setUIWindowSize(const glm::uvec2& size) override { m_NativeUIWindow->Size = ImVec2(size.x, size.y); }
		virtual void setUIWindowTitle(const string& title) override { m_Data.Title = title; }
		virtual void setUIWindowHide(const bool& hide = false) override;
		virtual void setCallbackFunction(const UICallbackFunction& callback) override { m_Data.Callback = callback; };

		virtual const glm::uvec2& getUIWindowPosition() override { return m_Data.Position; }
		virtual const glm::uvec2& getUIWindowSize() override { return m_Data.WindowSize; }
		virtual const glm::uvec2& getUIContentSize() override { return m_Data.ContentSize; }
		virtual const string& getUIWindowTitle() override { return m_Data.Title; }
		virtual ImGuiWindow* getNativeUIWindow() override { return m_NativeUIWindow; }

		virtual const bool& isMouseInside() override { return m_Data.MouseInside; }
		virtual const bool& isUIWindowFocused() override { return m_Data.Focus; }
		virtual const bool& isUIWindowVisibile() override { return m_Data.Hidden; }
	private:
		ImGuiWindow* m_NativeUIWindow = nullptr;

		struct UIData
		{
			string Title = "New Window";
			glm::uvec2 WindowSize, Position, ContentSize;
			bool Focus = true, Maximize = true, Hidden = false, MouseInside = false;

			UICallbackFunction Callback;
		} m_Data;

		bool m_FocusChanged = false, m_VisibilityChanged;
	};

	class ImUIWindowLayer : Window::IOLayer
	{
	public:
		ImUIWindowLayer(UIWindow* window)
			:m_UIWindow(window) {}

		virtual void OnIOUpdate() override { m_UIWindow->OnIOUpdate(); }
	private:
		UIWindow* m_UIWindow;
	};
}