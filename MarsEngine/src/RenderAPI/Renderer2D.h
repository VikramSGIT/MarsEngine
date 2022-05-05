#ifndef ME_RENDERER2D
#define ME_RENDERER2D

#pragma once

#include "MarsHeader.h"

#include "Shader2D.h"
#include "Modules\2D\Mesh2D.h"
#include "Window\Layers\Layer.h"
#include "RenderAPI\OpenGL\OpenGL2D\OpenGL2DShader.h"

namespace ME
{
    namespace Renderer
    {
        enum class Render2DAPItype
        {
            ME_NONE = 0,
            ME_RENDERER2D_OPENGL = 1
        };
        class Render2DAPI
        {
        public:
            Render2DAPI(Render2DAPItype api)
                :m_renderapi(api), m_Layer(this){}
            virtual ~Render2DAPI() = default;

            virtual void Init() = 0;
            virtual void Clear() const = 0;
            virtual void AddRenderSubmition(const ME::Ref<ME::MeshQueue>& meshqueue, std::function<void()> preprocessdata) = 0;

            virtual void OnEvent(Event::Event& e) = 0;
            virtual void OnUpdate() = 0;
            virtual void OnDraw() = 0;

            virtual bool SwitchAPI(const Render2DAPItype api) = 0;
            virtual void SetViewPortSize(const unsigned int& X, const unsigned int& Y) = 0;
            virtual void SetClearColor(const glm::vec4& color) = 0;
            virtual void SetShader(const Ref<Shader2D>& shader) = 0;

            inline Render2DAPItype GetAPI() { return m_renderapi; };
            virtual inline std::vector<ME::Ref<ME::MeshQueue>> GetRenderQueue() = 0;
            virtual Window::Layer::Layer& GetLayer() { return m_Layer; }


            Ref<Shader2D> Create(const std::string& filepath)
            {
                if (GetAPI() == Render2DAPItype::ME_RENDERER2D_OPENGL)
                {
                    return CreateRef<OpenGL::OpenGL2DShader>(filepath);
                }
                else
                {
                    ME_CORE_CRITICAL(true, "MarsEngine Only supports OpenGL for now!");
                    return nullptr;
                }
            }

        private:
            class Renderer2DLayer : public Window::Layer::Layer
            {
            public:
                Renderer2DLayer(Render2DAPI* api)
                    :Layer("Renderer2D"), m_API(api) {}

                virtual void OnAttach() override { m_API->Init(); }
                virtual void OnDetach() override { m_API->~Render2DAPI(); }
                virtual void OnDraw() override { m_API->OnDraw(); }
                virtual void OnUpdate(Timestep ts) override { m_API->OnUpdate(); }
                virtual void OnEvent(Event::Event&) override {}
            private:
                Render2DAPI* m_API;
            };

            Render2DAPItype m_renderapi;
            Renderer2DLayer m_Layer;
        };

    }
}

#endif