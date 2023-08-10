#ifndef ME_OPENGL2DRENDERER
#define ME_OPENGL2DRENDERER

#pragma once

#include "RenderAPI/Buffers.h"
#include "RenderAPI/Renderer.h"
#include "RenderAPI/Shader.h"
#include "Vender/glm/glm/glm.hpp"
#include "Vender/MTL/Vector.h"
#include "Vender/MTL/Array.h"
#include "OpenGLTexture2D.h"

#include "CUDAInterface/kernel.h"

#include  <vector>
#include <set>
namespace ME
{
    namespace OpenGL
    {
        struct RenderData2D
        {
            Ref<Shader> Shader;
            std::set<Mesh2D*> Update;
            Vector <Ref<Mesh2D>> Meshes;
            Ref<VertexbufferLayout> Layout;
            uint32_t vertex_id = 0, index_id = 0, NextIndex = 0, TextureSlotIndex = 1;
            Array<Ref<OpenGLTexture2D>, 32> TextureSlots;
        };
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

            virtual void AddMesh(const Ref<Mesh2D>& mesh) override; // FIX
            virtual void AddMesh(const std::vector<Ref<Mesh2D>>& meshes) override; // FIX
            virtual void AddMesh(const Ref<Mesh>& mesh) override { ME_CORE_ERROR(true, "3D Mesh submitted to 2D Renderer"); }
            virtual void AddMesh(const std::vector<Ref<Mesh>>& meshes) override { ME_CORE_ERROR(true, "3D Mesh submitted to 2D Renderer"); }
            virtual void AddFramebuffer(const FramebufferSpecification& framebufferspec) override {} // FIX

            virtual void PushUpdate(Mesh* mesh) override { ME_CORE_ERROR(true, "3D Mesh submitted to 2D Renderer"); };
            virtual void PushUpdate(Mesh2D* mesh) override; // FIX
            virtual void SetClearColor(const glm::vec4& color) override;
            virtual void SetShader(const Ref<Shader>& shader) override;
            virtual void SetViewPortSize(const unsigned int& X, const unsigned int& Y) override;

            virtual size_t GetTotalVertices() const override { return m_TotalVertices; }
            virtual size_t GetTotalIndices() const { return m_TotalIndices; }

        private:
            uint32_t m_TotalVertices, m_TotalIndices;
            glm::vec4 m_clearcolor;
            Vector<Ref<RenderData2D>> m_RenderData;
            CUDAMesh2DUpdate m_MeshUpdater;
        };
    }
}

#endif