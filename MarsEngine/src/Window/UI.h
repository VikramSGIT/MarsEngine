#pragma once

#include "MarsFlags.h"

#include "Events/Event.h"
#include "Core/Utilites/String.h"

#include "Vender/glm/glm/glm.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <functional>
namespace ME
{
	class UIWindow
	{
	public:

		using UICallbackFunction = std::function<void(Event::Event&)>;
		virtual void OnIOUpdate() = 0;

		virtual bool UIWindowBegin(const int&) = 0;
		virtual void UIWindowEnd() = 0;

		virtual void setUIWindowPosition(const glm::uvec2&) = 0;
		virtual void setUIWindowSize(const glm::uvec2&) = 0;
		virtual void setUIWindowTitle(const string&) = 0;
		virtual void setUIWindowHide(const bool& hide = false) = 0;
		virtual void setCallbackFunction(const UICallbackFunction&) = 0;

		virtual const glm::uvec2& getUIWindowPosition() = 0;
		virtual const glm::uvec2& getUIWindowSize() = 0;
		virtual const bool& getUIWindowFocus() = 0;
		virtual const bool& getUIWindowVisibility() = 0;
		virtual const string& getUIWindowTitle() = 0;
		virtual ImGuiWindow* getNativeUIWindow() = 0;
	};
}