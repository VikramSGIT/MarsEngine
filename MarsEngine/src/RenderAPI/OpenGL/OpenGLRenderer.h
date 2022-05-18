#ifndef ME_OPENGLRENDERER
#define ME_OPENGLRENDERER

#pragma once

#include "OpenGLShader.h"
#include "RenderAPI/Renderer.h"

#include <vector>

namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            class OpenGLRendererAPI : public RenderAPI
            {
            private:
                bool Ready = false, LiveStreamData = true;
                unsigned int indexoffset = 0;
                glm::vec4 m_clearcolor;
                Ref<OpenGLShader> m_Shader;
                std::vector<Ref<MeshQueue>> m_RenderQueue;
                std::vector <unsigned int> vertexbuffercache;
                std::vector<unsigned int> indexbuffercache;
                std::vector<std::function<void()>> preprocessing;

                void SetUpBuffers(const Ref<MeshQueue>& meshqueue);
                void CheckBufferUpdate(const unsigned int& id);
                void ClearBufferCache();
            public:
                OpenGLRendererAPI();
                ~OpenGLRendererAPI();

                virtual void Init() override;
                virtual void Clear() const override;
                virtual void AddRenderSubmition(const Ref<MeshQueue>& meshqueue, std::function<void()> preprocessdata) override;

                virtual void OnUpdate() override;
                virtual void OnDraw() override;
                virtual void OnEvent(Event::Event& e) override;

                virtual bool SwitchAPI(const RenderAPItype api);

                virtual void SetClearColor(const glm::vec4& color) override;
                virtual void SetShader(const Ref<Shader>& shader) override;
                virtual void SetViewPortSize(const unsigned int& X, const unsigned int& Y) override;
                virtual inline std::vector<Ref<MeshQueue>> GetRenderQueue() override { return m_RenderQueue; }
            };

        }
    }
}

#endif