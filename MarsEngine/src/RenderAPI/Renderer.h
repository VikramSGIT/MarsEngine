#pragma once
#include "MarsHeader.h"
#include "Shader.h"

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
        virtual void SetClearColor(const oglm::vec4& color) = 0;
        virtual Ref<Layer::BasicLayer> GetLayer() = 0;
        virtual void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const = 0;

        virtual bool SwitchAPI(const RenderAPItype api) = 0;

        virtual RenderAPItype GetAPI() { return m_renderapi; };

        Ref<VertexBuffer> Create(const void* data, const unsigned int size, const unsigned int mode)
        {

            ME_PROFILE_TRACE_CALL();

            if (GetAPI() == RenderAPItype::ME_RENDERER_OPENGL)
            {
                return CreateRef<OpenGL::OpenGLVertexBuffer>(data, size, mode);
            }
            else
            {
                ME_CORE_ERROR("MarsEngine Only supports OpenGl!");
                return nullptr;
            }
        }

        Ref<IndexBuffer> Create(const unsigned int* data, const unsigned int size, const unsigned int mode)
        {

            ME_PROFILE_TRACE_CALL();

            if (GetAPI() == RenderAPItype::ME_RENDERER_OPENGL)
            {
                return CreateRef<OpenGL::OpenGLIndexBuffer>(data, size, mode);
            }
            else
            {
                ME_CORE_ERROR("MarsEngine Only supports OpenGl!");
                return nullptr;
            }
        }

        Ref<VertexArray> Create()
        {

            ME_PROFILE_TRACE_CALL();

            if (GetAPI() == RenderAPItype::ME_RENDERER_OPENGL)
            {
                return CreateRef<OpenGL::OpenGLVertexArray>();
            }
            else
            {
                ME_CORE_ERROR("MarsEngine Only supports OpenGl!");
                return nullptr;
            }
        }

        Ref<VertexBufferLayout> CreateBufferLayout()
        {

            ME_PROFILE_TRACE_CALL();

            if (GetAPI() == RenderAPItype::ME_RENDERER_OPENGL)
            {
                return CreateRef<VertexBufferLayout>();
            }
            else
            {
                ME_CORE_ERROR("MarsEngine Only supports OpenGl!");
                    return nullptr;
            }
        }

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