#ifndef ME_RENDERER
#define ME_RENDERER

#pragma once

#define ME_OPENGL_MAXVERTEXBUFFER 1000
#define ME_OPENGL_MAXINDEXBUFFER 1000

#include "Shader.h"
#include "Modules/Mesh.h"
#include "Modules/2D/Mesh2D.h"
#include "Window/Layers/Layer.h"
#include "Buffers.h"
#include "Vender/MTL/Ref.h"

namespace ME
{
    enum class RenderAPItype
    {
        ME_NONE = 0,
        ME_RENDERER_OPENGL,
        ME_RENDERER_OPENGL2D
    };

    class RenderAPI : public Window::Layer
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

        virtual void AddMesh(const Ref<Mesh>&) = 0;
        virtual void AddMesh(const std::vector<Ref<Mesh>>&) = 0;
        virtual void AddMesh(const Ref<Mesh2D>&) = 0;
        virtual void AddMesh(const std::vector<Ref<Mesh2D>>&) = 0;
        virtual void AddFramebuffer(const FramebufferSpecification& framebufferspec) = 0;

        virtual void PushUpdate(Mesh* mesh) = 0;
        virtual void PushUpdate(Mesh2D* mesh) = 0;
        virtual void SetViewPortSize(const uint32_t& X, const uint32_t& Y) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void SetShader(const Ref<Shader>& shader) = 0;

        virtual size_t GetTotalVertices() const = 0;
        virtual size_t GetTotalIndices() const = 0;

        static RenderAPItype GetAPI() { return s_renderapi; }
        static Ref<RenderAPI> Create(const RenderAPItype& renderapitype);

        Ref<Framebuffer> framebuffer;
    private:
        static RenderAPItype s_renderapi;
    };
}

#endif