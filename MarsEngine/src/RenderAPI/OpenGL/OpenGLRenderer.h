#pragma once
#include "MarsHeader.h"
#include "RenderAPI/Buffers.h"

#include "OpenGLVertexArray.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLShader.h"
#include "Window/Layers/BasicLayer.h"
#include "Vender/OGLM/oglm.h"
#include "RenderAPI/Renderer.h"
#include "GL/glew.h"
#include "OpenGLErrorhandle.h"
#include "Core/Logger.h"

#include <iostream>
#include <sstream>
#include <algorithm>

namespace Renderer
{
    namespace OpenGL
    {
        class OpenGLRendererAPI : public RenderAPI 
        {
        private:
            bool Ready = false, LiveStreamData = true;
            unsigned int indexoffset = 0;
            oglm::vec4<float> m_clearcolor;
            std::vector<MeshQueue> m_RenderQueue;
            Scope<VertexBuffer> vertex = CreateScope<OpenGLVertexBuffer>(ME_MAX_VERTEX_BUFFER_SIZE, GL_DYNAMIC_DRAW);
            Scope<IndexBuffer> index = CreateScope<OpenGLIndexBuffer>(ME_MAX_INDEX_BUFFER_SIZE, GL_DYNAMIC_DRAW);
        public:
            OpenGLRendererAPI();
            ~OpenGLRendererAPI();

            void Init() override;
            void OnUpdate() override;
            void OnEvent(Event::Event& e);
            void Clear() const;
            void AddRenderSubmition(const MeshQueue& meshqueue) override;

            bool SwitchAPI(const RenderAPItype api);
            void SetViewPortSize(const unsigned int& X, const unsigned int& Y) override;
            void SetClearColor(const oglm::vec4<float>& color) override;
            void Draw(const Shader& shader) override;

            inline Ref<Layer::BasicLayer> GetLayer() override;
            inline std::vector<MeshQueue> GetRenderQueue() override { return m_RenderQueue; }
        };

    }
}