#pragma once
#include "MarsHeader.h"
#include "Shader.h"
#include "Mesh.h"

namespace Renderer
{
    enum class RenderAPItype
    {
        ME_NONE = 0,
        ME_RENDERER_OPENGL = 1
    };
    class RenderAPI
    {
    private:
        RenderAPItype m_renderapi;
    public:
        RenderAPI(RenderAPItype api)
            :m_renderapi(api) {}
        virtual ~RenderAPI() = default;

        virtual void Init() = 0;
        virtual void OnUpdate() = 0;
        virtual void SetClearColor(const oglm::vec4<float>& color) = 0;
        virtual void AddRenderSubmition(const MeshQueue& meshqueue) = 0;
        virtual Ref<Layer::BasicLayer> GetLayer() = 0;
        virtual void Draw(const Shader& shader) = 0;

        virtual bool SwitchAPI(const RenderAPItype api) = 0;
        virtual void SetViewPortSize(const unsigned int& X, const unsigned int& Y) = 0;

        inline RenderAPItype GetAPI() { return m_renderapi; };
        virtual inline std::vector<MeshQueue> GetRenderQueue() = 0;


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
    };

}