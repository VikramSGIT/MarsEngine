#ifndef ME_RENDERER
#define ME_RENDERER

#pragma once

#include "Shader.h"
#include "Modules/Mesh.h"
#include "Modules/2D/Mesh2D.h"
#include "Window/Layers/Layer.h"

namespace ME
{
    namespace Renderer
    {
        enum class RenderAPItype
        {
            ME_NONE = 0,
            ME_RENDERER_OPENGL,
            ME_RENDERER_OPENGL2D
        };
        class RenderAPI : public Window::Layer::Layer
        {
        public:
            RenderAPI(RenderAPItype api)
                :Layer("Renderer") {}
            virtual ~RenderAPI() = default;

            virtual void OnAttach() override = 0;
            virtual void OnDetach() override = 0;
            virtual void OnEvent(Event::Event& e) override = 0;
            virtual void OnUpdate(Timestep ts) override = 0;
            virtual void OnDraw() override = 0;

            virtual void AddRenderSubmition(const Ref<MeshQueue>&, std::function<void()>) = 0;
            virtual void AddRenderSubmition(const Ref<MeshQueue2D>&, std::function<void()>) = 0;
            virtual void SetViewPortSize(const uint32_t& X, const uint32_t& Y) = 0;
            virtual void SetClearColor(const glm::vec4& color) = 0;
            virtual void SetShader(const Ref<Shader>& shader) = 0;

            static RenderAPItype GetAPI() { return s_renderapi; }
            static Ref<RenderAPI> Create(const RenderAPItype& renderapitype);

        private:
            static RenderAPItype s_renderapi;
        };

    }
}

#endif