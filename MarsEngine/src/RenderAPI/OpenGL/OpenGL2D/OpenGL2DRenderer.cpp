#include "MarsHeader.h"
#include "OpenGL2DRenderer.h"

#include "Core/Logger.h"
#include "RenderAPI/OpenGL/OpenGLErrorhandler.h"
#include "RenderAPI/OpenGL/OpenGLShader.h"

#include "GL/glew.h"
#include <algorithm>

namespace ME
{
    namespace OpenGL
    {
        OpenGL2DRendererAPI::OpenGL2DRendererAPI()
            :RenderAPI(RenderAPItype::ME_RENDERER_OPENGL2D), m_clearcolor(glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f }), 
            m_TotalVertices(0), m_TotalIndices(0)
        {

            ME_PROFILE_TRACE_CALL();

            Ref<RenderData2D> data = CreateRef<RenderData2D>();

            data->Layout = CreateRef<VertexbufferLayout>();
            data->Layout->push(GL_FLOAT, 2);
            data->Layout->push(GL_FLOAT, 2);
            data->Layout->push(GL_FLOAT, 1);

            m_RenderData.push_back(data);
        }

        OpenGL2DRendererAPI::~OpenGL2DRendererAPI()
        {

            ME_PROFILE_TRACE_CALL();

        }

        void OpenGL2DRendererAPI::SetViewPortSize(const unsigned int& X, const unsigned int& Y)
        {
            glViewport(0, 0, X, Y);
        }

        void OpenGL2DRendererAPI::PushUpdate(Mesh2D* mesh)
        {
            m_RenderData[0]->Update.insert(mesh);
        }

        void OpenGL2DRendererAPI::SetClearColor(const glm::vec4& color)
        {

            ME_PROFILE_TRACE_CALL();

            m_clearcolor = color;
        }

        void OpenGL2DRendererAPI::SetShader(const Ref<Shader>& shader)
        {

            ME_PROFILE_TRACE_CALL();

            m_RenderData[0]->Shader = shader;
        }

        void OpenGL2DRendererAPI::OnAttach()
        {

            ME_PROFILE_TRACE_CALL();
//
// Initiating graphics libraries
// Need to add graphics drivers identification
            ME_CORE_CRITICAL(glewInit() != GLEW_OK, "Can't Impliment GLEW")
#ifdef ME_DEBUG
            ME_CORE_INFO("Detected OpenGL Version (using) : {}", glGetString(GL_VERSION));
#endif
//
// Enabling blending, typically transparencies
//
            GLLogCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            GLLogCall(glEnable(GL_BLEND));

            GLLogCall(glGenBuffers(1, &m_RenderData[0]->vertex_id));
            GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderData[0]->vertex_id));
            GLLogCall(glBufferData(GL_ARRAY_BUFFER, ME_OPENGL_MAXVERTEXBUFFER * sizeof(VERTEX2D), nullptr, GL_DYNAMIC_DRAW));
            m_MeshUpdater.BindVertexBuffer(m_RenderData[0]->vertex_id);

            GLLogCall(glGenBuffers(1, &m_RenderData[0]->index_id));
            GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderData[0]->index_id));
            GLLogCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, ME_OPENGL_MAXINDEXBUFFER * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW));

            GLLogCall(glBindVertexArray(1));

            unsigned int offset = 0;

            auto& elements = m_RenderData[0]->Layout->GetElements();

            for (unsigned int j = 0; j < elements.size(); j++)
            {
                auto element = elements[j];
                GLLogCall(glEnableVertexAttribArray(j));
                GLLogCall(glVertexAttribPointer(j, element.count, element.type, element.normalized, m_RenderData[0]->Layout->GetStride(), (const void*)offset));
                offset += GetElementSize(element.type) * element.count;
            }
        }

        void OpenGL2DRendererAPI::OnDetach()
        {
            for (auto data : m_RenderData)
            {
                GLLogCall(glDeleteBuffers(1, &data->vertex_id));
                GLLogCall(glDeleteBuffers(1, &data->index_id));
            }
        }

        void OpenGL2DRendererAPI::OnUpdate(Timestep ts)
        {

            ME_PROFILE_TRACE_CALL();

            if (m_RenderData[0]->Update.size() == 0) return;

            MeshData2D* data = alloc<MeshData2D>(m_RenderData[0]->Update.size());
            size_t offset = 0;
            for (Mesh2D* mesh : m_RenderData[0]->Update)
                data[offset++] = *mesh->getMeshData();

            m_MeshUpdater.UpdateMeshes(data, m_RenderData[0]->Update.size());

            dealloc(data);

            m_RenderData[0]->Update.clear();

            GLLogCall(glClearColor(m_clearcolor.x, m_clearcolor.y, m_clearcolor.z, m_clearcolor.w));
            GLLogCall(glClear(GL_COLOR_BUFFER_BIT));
        }

        void OpenGL2DRendererAPI::OnDraw()
        {

            ME_PROFILE_TRACE_CALL();
//
// Will Soon Sort out the shaders when Materials Are Implemented
//
// Setting up VertexArray for each call, needs to be fixed at higher builds!!
//          
            for (auto data : m_RenderData)
            {

                GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, data->vertex_id));
                GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->index_id));

                if (framebuffer.IsValid())
                    framebuffer->Bind();
                data->Shader->Bind();

                GLLogCall(glDrawElements(GL_TRIANGLES, m_TotalIndices, GL_UNSIGNED_INT, nullptr));
            }
            if (framebuffer.IsValid())
                framebuffer->unBind();
        }

        void OpenGL2DRendererAPI::OnEvent(Event::Event& e)
        {

            ME_PROFILE_TRACE_CALL();

        }
        void OpenGL2DRendererAPI::AddMesh(const Ref<Mesh2D>& mesh)
        {
            float textureindex;

            for (uint32_t i = 0; i < m_RenderData[0]->TextureSlotIndex; i++)
                if (m_RenderData[0]->TextureSlots[i] == mesh->getTexture())
                {
                    textureindex = (float)i;
                    break;
                }

            if (textureindex == 0.0f && mesh->getTexture().IsValid())
            {
            }

            Ref<Mesh2D> ms = mesh;
            ms->getPrimitive()->m_VertexOffset = ms->getMeshData()->vertexoffset = m_TotalVertices;
            ms->getPrimitive()->m_IndexOffset = m_TotalIndices;
            ms->getPrimitive()->m_StartIndex = m_RenderData[0]->NextIndex;

            m_RenderData[0]->Meshes.emplace_back(ms);

            GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderData[0]->vertex_id));
            GLLogCall(glBufferSubData(GL_ARRAY_BUFFER, m_TotalVertices * sizeof(VERTEX2D),
                ms->getPrimitive()->vertex.size() * sizeof(VERTEX2D),
                ms->getPrimitive()->vertex.begin()));

            vector<VERTEX2D>& ver = ms->getPrimitive()->vertex;

            m_MeshUpdater.BufferMeshPrimitive(ms->getPrimitive()->vertex.begin(), m_TotalVertices, ms->getPrimitive()->vertex.size());

            unsigned int max = 0;
            unsigned int* local = alloc<unsigned int>(ms->getPrimitive()->index.size());
            for (unsigned int i = 0; i < ms->getPrimitive()->index.size(); i++)
            {
                unsigned int j = ms->getPrimitive()->index[i];
                max < j ? max = j : max;
                local[i] = j + m_RenderData[0]->NextIndex;
            }

            GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderData[0]->index_id));
            GLLogCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_TotalIndices * sizeof(unsigned int),
                ms->getPrimitive()->index.size() * sizeof(unsigned int), local));

            dealloc(local);

            m_RenderData[0]->NextIndex += (max + 1);
            m_TotalVertices += ms->getPrimitive()->vertex.size();
            m_TotalIndices += ms->getPrimitive()->index.size();

            ms->getPrimitive()->vertex.clear();
            ms->getPrimitive()->index.clear();
        }
        void OpenGL2DRendererAPI::AddMesh(const std::vector<Ref<Mesh2D>>& meshes)
        {
            for (Ref<Mesh2D> ms : meshes)
            {
                ms->getPrimitive()->m_VertexOffset = ms->getMeshData()->vertexoffset = m_TotalVertices;
                ms->getPrimitive()->m_IndexOffset = m_TotalIndices;
                ms->getPrimitive()->m_StartIndex = m_RenderData[0]->NextIndex;

                m_RenderData[0]->Meshes.emplace_back(ms);

                GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderData[0]->vertex_id));
                GLLogCall(glBufferSubData(GL_ARRAY_BUFFER, m_TotalVertices * sizeof(VERTEX2D),
                    ms->getPrimitive()->vertex.size() * sizeof(VERTEX2D),
                    ms->getPrimitive()->vertex.begin()));

                m_MeshUpdater.BufferMeshPrimitive(ms->getPrimitive()->vertex.begin(), m_TotalVertices, ms->getPrimitive()->vertex.size());

                unsigned int max = 0;
                unsigned int* local = alloc<unsigned int>(ms->getPrimitive()->index.size());
                for (unsigned int i = 0; i < ms->getPrimitive()->index.size(); i++)
                {
                    unsigned int j = ms->getPrimitive()->index[i];
                    max < j ? max = j : max;
                    local[i] = j + m_RenderData[0]->NextIndex;
                }

                GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderData[0]->index_id));
                GLLogCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_TotalIndices * sizeof(unsigned int),
                    ms->getPrimitive()->index.size() * sizeof(unsigned int), local));

                dealloc(local);

                m_RenderData[0]->NextIndex += (max + 1);
                m_TotalVertices += ms->getPrimitive()->vertex.size();
                m_TotalIndices += ms->getPrimitive()->index.size();

                ms->getPrimitive()->vertex.clear();
                ms->getPrimitive()->index.clear();
            }
        }
    }
}