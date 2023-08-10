#ifndef ME_OPENGLRENDERER
#define ME_OPENGLRENDERER

#pragma once

#include "OpenGLShader.h"
#include "RenderAPI/Renderer.h"
#include "Vender/MTL/Ref.h"
#include "Vender/MTL/Vector.h"

namespace ME
{
    namespace OpenGL
    {
        class OpenGLRendererAPI : public RenderAPI
        {
        public:
            OpenGLRendererAPI();
            ~OpenGLRendererAPI();

            virtual void AddFramebuffer(const FramebufferSpecification& framebuffer);
            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnUpdate(Timestep ts) override;
            virtual void OnDraw() override;
            virtual void OnEvent(Event::Event& e) override;

            virtual void AddMesh(const Ref<Mesh2D>& mesh) override;
            virtual void AddMesh(const std::vector<Ref<Mesh2D>>& meshes) override;
            virtual void AddMesh(const Ref<Mesh>& mesh) override;
            virtual void AddMesh(const std::vector<Ref<Mesh>>& meshes) override;

            virtual void PushUpdate(Mesh* mesh) override;
            virtual void PushUpdate(Mesh2D* mesh) override;
            virtual void SetClearColor(const glm::vec4& color) override;
            virtual void SetShader(const Ref<Shader>& shader) override;
            virtual void SetViewPortSize(const unsigned int& X, const unsigned int& Y) override;

            virtual size_t GetTotalVertices() const override { return m_TotalVertices; }
            virtual size_t GetTotalIndices() const override { return m_TotalIndices; }
        private:
            unsigned int vertex_id, index_id, m_TotalIndices, m_TotalVertices, m_NextIndex;
            FramebufferSpecification m_FrameSpec;
            glm::vec4 m_clearcolor;
            Ref<Shader> m_Shader;
            Vector <Ref<Mesh>> m_Meshes, m_Mesh2D;
            Vector <Mesh*> m_Update;
            Vector <Mesh2D*> m_Update2D;
            Ref<VertexbufferLayout> m_Layout;
        };

    }
}

#endif