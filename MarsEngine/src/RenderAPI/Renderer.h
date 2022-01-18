#pragma once
#include "MarsHeader.h"

#include "Shader.h"
#include "Mesh.h"
#include "Window/Layers/Layer.h"
#include "RenderAPI/OpenGL/OpenGLShader.h"

namespace ME
{
    namespace Renderer
    {
        enum class RenderAPItype
        {
            ME_NONE = 0,
            ME_RENDERER_OPENGL = 1
        };
        class RenderAPI
        {
        public:
            RenderAPI(RenderAPItype api)
                :m_renderapi(api), m_Layer(this){}
            virtual ~RenderAPI() = default;

            virtual void Init() = 0;
            virtual void Clear() const = 0;
            virtual void AddRenderSubmition(const ME::Ref<ME::MeshQueue>& meshqueue, std::function<void()> preprocessdata) = 0;

            virtual void OnEvent(Event::Event& e) = 0;
            virtual void OnUpdate() = 0;
            virtual void OnDraw() = 0;

            virtual bool SwitchAPI(const RenderAPItype api) = 0;
            virtual void SetViewPortSize(const unsigned int& X, const unsigned int& Y) = 0;
            virtual void SetClearColor(const glm::vec4& color) = 0;
            virtual void SetShader(const Ref<Shader>& shader) = 0;

            inline RenderAPItype GetAPI() { return m_renderapi; };
            virtual inline std::vector<ME::Ref<ME::MeshQueue>> GetRenderQueue() = 0;
            virtual Window::Layer::Layer& GetLayer() { return m_Layer; }


            Ref<Shader> Create(const std::string& filepath)
            {
                if (GetAPI() == RenderAPItype::ME_RENDERER_OPENGL)
                {
                    return CreateRef<OpenGL::OpenGLShader>(filepath);
                }
                else
                {
                    ME_CORE_ERROR("MarsEngine Only supports OpenGl!");
                    return nullptr;
                }
            }

        private:
            class RendererLayer : public Window::Layer::Layer
            {
            public:
                RendererLayer(RenderAPI* api)
                    :Layer("Renderer"), m_API(api) {}

                virtual void OnAttach() override { m_API->Init(); }
                virtual void OnDetach() override { m_API->~RenderAPI(); }
                virtual void OnDraw() override { m_API->OnDraw(); }
                virtual void OnUpdate(Timestep ts) override { m_API->OnUpdate(); }
                virtual void OnEvent(Event::Event&) override {}
            private:
                RenderAPI* m_API;
            };

            RenderAPItype m_renderapi;
            RendererLayer m_Layer;
        };

    }
}