#include "imguiLayer.h"

#include "Vender/imgui/imgui.h"
#include "Window/Windows/WindowsWindow.h"
#include "Vender/GLFW/glfw3.h"
#include "Vender/imgui/imgui.h"
#include "Vender/imgui/backends/imgui_impl_opengl3.h"
#include "Vender/imgui/backends/imgui_impl_glfw.h"
#include "Core/Application.h"
namespace ME
{
    namespace Window
    {
        namespace Layer
        {
            imguiLayer::imguiLayer()
                :Layer("ImGUI"), m_Time(0)
            {

                ME_PROFILE_TRACE_CALL();

            }

            imguiLayer::~imguiLayer()
            {

                ME_PROFILE_TRACE_CALL();

            }

            void imguiLayer::OnAttach()
            {

                ME_PROFILE_TRACE_CALL();

                Application& app = Application::GetInstance();

                // Setup Dear ImGui context
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO(); (void)io;
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
                //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
                io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
                io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
                //io.ConfigViewportsNoAutoMerge = true;
                //io.ConfigViewportsNoTaskBarIcon = true;

                // Setup Dear ImGui style
                //ImGui::StyleColorsDark();
                ImGui::StyleColorsClassic();

                ImGuiStyle& style = ImGui::GetStyle();
                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                {
                    style.WindowRounding = 0.0f;
                    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
                }

                ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow()), true);
                ImGui_ImplOpenGL3_Init("#version 400");
            }
            void imguiLayer::OnDetach()
            {

                ME_PROFILE_TRACE_CALL();

                ImGui_ImplGlfw_Shutdown();
                ImGui_ImplOpenGL3_Shutdown();
                ImGui::DestroyContext();
            }

            void imguiLayer::OnUpdate(Timestep ts)
            {

                ME_PROFILE_TRACE_CALL();

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
                ImGuiIO& io = ImGui::GetIO();

                draw();

                ImGui::Render();

                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                // Update and Render additional Platform Windows
                // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
                //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                {
                    GLFWwindow* backup_current_context = glfwGetCurrentContext();
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                    glfwMakeContextCurrent(backup_current_context);
                }
            }

            void imguiLayer::OnEvent(Event::Event& e)
            {

                ME_PROFILE_TRACE_CALL();

                if (m_BlockEvents)
                {
                    ImGuiIO& io = ImGui::GetIO();
                    e.Handled |= e.IsInCategory(Event::EventCategoryFlag::EventCategoryMouse) & io.WantCaptureMouse;
                    e.Handled |= e.IsInCategory(Event::EventCategoryFlag::EventCategoryKeyboard) & io.WantCaptureKeyboard;
                }
            }

            void imguiLayer::SetDrawData(const std::function<void()>& func)
            {
                draw = func;
            }
        }
    }
}


        
  