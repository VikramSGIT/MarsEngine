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

namespace Renderer
{
    namespace OpenGL
    {
        class OpenGLRendererAPI : public RenderAPI 
        {
        private:
            oglm::vec4 m_clearcolor;
        public:
            OpenGLRendererAPI();
            ~OpenGLRendererAPI();

            void Init() override;
            void OnUpdate() override;
            void OnEvent(Event::Event& e);
            void Clear() const;

            bool SwitchAPI(const RenderAPItype api);

            void SetClearColor(const oglm::vec4& color) override;
            void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const override;

            Ref<Layer::BasicLayer> GetLayer() override;
        };

    }
}