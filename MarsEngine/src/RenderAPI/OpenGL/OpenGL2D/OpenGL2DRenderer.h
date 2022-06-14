#ifndef ME_OPENGL2DRENDERER
#define ME_OPENGL2DRENDERER

#pragma once

#include "RenderAPI/Buffers.h"
#include "RenderAPI/Renderer.h"
#include "RenderAPI/Shader.h"
#include "Vender/glm/glm/glm.hpp"
#include "Core/Utilites/Vector.h"

namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            class OpenGL2DRendererAPI : public RenderAPI
            {
            public:
                OpenGL2DRendererAPI();
                ~OpenGL2DRendererAPI();

                virtual void OnAttach() override;
                virtual void OnDetach() override;
                virtual void OnUpdate(Timestep ts) override;
                virtual void OnDraw() override;
                virtual void OnEvent(Event::Event& e) override;

                virtual void AddMesh(const Ref<Mesh2D>& mesh) override;
                virtual void AddMesh(const std::vector<Ref<Mesh2D>>& meshes) override;
                virtual void AddMesh(const Ref<Mesh>& mesh) override { ME_CORE_ERROR(true, "3D Mesh submitted to 2D Renderer"); }
                virtual void AddMesh(const std::vector<Ref<Mesh>>& meshes) override { ME_CORE_ERROR(true, "3D Mesh submitted to 2D Renderer"); }
                virtual void AddFramebuffer(const FramebufferSpecification& framebufferspec) override {}

                virtual void PushUpdate(Mesh* mesh) override { ME_CORE_ERROR(true, "3D Mesh submitted to 2D Renderer"); };
                virtual void PushUpdate(Mesh2D* mesh) override;
                virtual void SetClearColor(const glm::vec4& color) override;
                virtual void SetShader(const Ref<Shader>& shader) override;
                virtual void SetViewPortSize(const unsigned int& X, const unsigned int& Y) override;

            private:
                unsigned int vertex_id, index_id, m_TotalVertices, m_TotalIndices, m_NextIndex;
                FramebufferSpecification spec;
                glm::vec4 m_clearcolor;
                Ref<Shader> m_Shader;
                Vector <Mesh2D*> m_Update;
                Vector <Ref<Mesh2D>> m_Meshes;
                Ref<VertexbufferLayout> m_Layout;
            };

        }
    }
}

#endif