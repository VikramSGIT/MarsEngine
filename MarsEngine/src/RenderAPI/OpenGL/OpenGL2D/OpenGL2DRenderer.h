#ifndef ME_OPENGL2DRENDERER
#define ME_OPENGL2DRENDERER

#pragma once

#include "MarsHeader.h"
#include "RenderAPI/Buffers.h"

#include "RenderAPI/Renderer2D.h"

#include "RenderAPI\OpenGL\OpenGLVertexArray.h"
#include "RenderAPI\OpenGL\OpenGLIndexBuffer.h"
#include "RenderAPI\OpenGL\OpenGLShader.h"
#include "Window/Layers/BasicLayer.h"
#include "Vender/glm/glm/glm.hpp"
#include "GL/glew.h"
#include "RenderAPI\OpenGL\OpenGLErrorhandler.h"
#include "Core/Logger.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            class OpenGL2DRendererAPI : public Render2DAPI
            {
            private:
                bool Ready = false, LiveStreamData = true;
                unsigned int indexoffset = 0;
                glm::vec4 m_clearcolor;
                Ref<Shader> m_Shader;
                std::vector<Ref<MeshQueue2D>> m_RenderQueue;
                std::vector <unsigned int> vertexbuffercache;
                std::vector<unsigned int> indexbuffercache;
                std::vector<std::function<void()>> preprocessing;

                void SetUpBuffers(const Ref<MeshQueue2D>& meshqueue);
                void CheckBufferUpdate(const unsigned int& id);
                void ClearBufferCache();
            public:
                OpenGL2DRendererAPI();
                ~OpenGL2DRendererAPI();

                virtual void Init() override;
                virtual void Clear() const override;
                virtual void AddRenderSubmition(const Ref<MeshQueue2D>& meshqueue, std::function<void()> preprocessdata) override;

                virtual void OnUpdate() override;
                virtual void OnDraw() override;
                virtual void OnEvent(Event::Event& e) override;

                virtual bool SwitchAPI(const Render2DAPItype api);

                virtual void SetClearColor(const glm::vec4& color) override;
                virtual void SetShader(const Ref<Shader>& shader) override;
                virtual void SetViewPortSize(const unsigned int& X, const unsigned int& Y) override;
                virtual inline std::vector<Ref<MeshQueue2D>> GetRenderQueue() override { return m_RenderQueue; }
            };

        }
    }
}

#endif