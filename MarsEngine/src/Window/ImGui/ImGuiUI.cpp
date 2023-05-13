#include "MarsHeader.h"
#include "ImGuiUI.h"

#include "Window/Events/UIEvent.h"
#include "Core/Application.h"
namespace ME
{
    void ImGuiUIWindow::OnIOUpdate()
    {
        ME_CORE_ERROR(m_NativeUIWindow == nullptr, "UIWindow not initialised properly!!");

        glm::uvec2 pos{ m_NativeUIWindow->Pos.x, m_NativeUIWindow->Pos.y };
        glm::uvec2 windowsize{ m_NativeUIWindow->Size.x, m_NativeUIWindow->Size.y };
        glm::uvec2 contentsize{m_NativeUIWindow->ContentSize.x, m_NativeUIWindow->ContentSize.y};

        if (m_Data.Position != pos)
        {
            m_Data.Position = pos;
            Event::UIWindowMoveEvent event(this, pos);
            m_Data.Callback(event);
        }

        if (m_Data.WindowSize != windowsize || m_Data.ContentSize != contentsize)
        {
            m_Data.WindowSize = windowsize;
            m_Data.ContentSize = contentsize;
            Event::UIWindowResizeEvent event(this, windowsize, contentsize);
            m_Data.Callback(event);
        }

        if (m_FocusChanged)
        {
            if (m_Data.Focus)
            {
                Event::UIWindowFocusEvent event(this);
                m_Data.Callback(event);
            }
            else
            {
                Event::UIWindowLostFocusEvent event(this);
                m_Data.Callback(event);
            }
            m_FocusChanged = false;
        }

        if (m_VisibilityChanged)
        {
            if (m_Data.Hidden)
            {
                Event::UIWindowHideEvent event(this);
                m_Data.Callback(event);
            }
            else
            {
                Event::UIWindowShowEvent event(this);
                m_Data.Callback(event);
            }
            m_VisibilityChanged = false;
        }

    }
    bool ImGuiUIWindow::UIWindowBegin(const int& flags)
    {
        bool visibility = m_Data.Hidden;
        bool res = ImGui::Begin(m_Data.Title.c_str(), &visibility, flags);
        m_NativeUIWindow = ImGui::GetCurrentWindow();

        if (m_Data.Focus != ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
        {
            m_Data.Focus = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
            m_FocusChanged = true;
        }

        if (visibility != m_Data.Hidden)
        {
            m_Data.Hidden = visibility;
            m_VisibilityChanged = true;
        }
        return res;
    }
    void ImGuiUIWindow::UIWindowEnd()
    {
        ImGui::End();
    }
    void ImGuiUIWindow::setUIWindowHide(const bool& hide)
    {
        if (m_Data.Hidden != hide)
        {
            m_Data.Hidden = hide;
            m_VisibilityChanged = true;
        }
    }
}