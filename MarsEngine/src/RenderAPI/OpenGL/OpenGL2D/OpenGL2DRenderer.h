#ifndef ME_OPENGL2DRENDERER
#define ME_OPENGL2DRENDERER

#pragma once

#include "MarsHeader.h"
#include "RenderAPI/Buffers.h"

#include "RenderAPI/Renderer.h"

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
            class OpenGL2DRendererAPI : public RenderAPI
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

                virtual void AddRenderSubmition(const Ref<MeshQueue2D>& meshqueue, std::function<void()> preprocessdata) override;
                virtual void AddRenderSubmition(const Ref<MeshQueue>& meshqueue, std::function<void()> preprocessdata) override { ME_CORE_ERROR(true, "3D MeshQueue submitted to 2D Renderer"); }

                virtual void OnAttach() override;
                virtual void OnDetach() override;
                virtual void OnUpdate(Timestep ts) override;
                virtual void OnDraw() override;
                virtual void OnEvent(Event::Event& e) override;

                virtual void SetClearColor(const glm::vec4& color) override;
                virtual void SetShader(const Ref<Shader>& shader) override;
                virtual void SetViewPortSize(const unsigned int& X, const unsigned int& Y) override;
            };

        }
    }
}

#endif