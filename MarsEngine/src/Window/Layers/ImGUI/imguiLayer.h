#pragma once
#include "MarsHeader.h"

#include "Vender/imgui/imgui.h"
#include "Window/Layers/Layer.h"
#include "Window/Windows/WindowsWindow.h"
#include "Vender/GLFW/glfw3.h"
#include "Vender/imgui/imgui.h"
#include "Vender/imgui/backends/imgui_impl_opengl3.h"
#include "Vender/imgui/backends/imgui_impl_glfw.h"
#include "Core/Application.h"


#include <memory>
#include <functional>

namespace Window
{
    namespace Layer
    {
        class imguiLayer : public Layer
        {
        private:
            float m_Time;
            bool m_BlockEvents = true;
            bool m_Show = true;
            std::function<void()> draw;

        public:
            imguiLayer();
            ~imguiLayer();

            void OnAttach() override;
            void OnDetach() override;
            void OnUpdate() override;
            void OnEvent(Event::Event& e);
            void SetDrawData(const std::function<void()>& func);

            void SetBlockEvents(bool block) { m_BlockEvents = !block;}
        };

    }
}
        