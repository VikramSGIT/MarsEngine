#pragma once
#include "MarsHeader.h"
#include "Shader.h"
#include "Mesh.h"
#include "Window/Layers/Layer.h"
#include "Window/Layers/BasicLayer.h"
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
        private:
            RenderAPItype m_renderapi;
        public:
            RenderAPI(RenderAPItype api)
                :m_renderapi(api) {}
            virtual ~RenderAPI() = default;

            virtual void Init() = 0;
            virtual void OnUpdate() = 0;
            virtual void OnEvent(Event::Event& e) = 0;
            virtual void Clear() const = 0;
            virtual void SetClearColor(const glm::vec4& color) = 0;
            virtual void AddRenderSubmition(const ME::MeshQueue& meshqueue, std::function<void()> preprocessdata) = 0;
            virtual Ref<Layer::BasicLayer> GetLayer() = 0;
            virtual void Draw(const Ref<Shader>& shader) = 0;

            virtual bool SwitchAPI(const RenderAPItype api) = 0;
            virtual void SetViewPortSize(const unsigned int& X, const unsigned int& Y) = 0;

            inline RenderAPItype GetAPI() { return m_renderapi; };
            virtual inline std::vector<ME::MeshQueue> GetRenderQueue() = 0;


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
}