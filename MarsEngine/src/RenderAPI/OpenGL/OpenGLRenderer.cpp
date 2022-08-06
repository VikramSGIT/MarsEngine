#include "MarsHeader.h"
#include "OpenGLRenderer.h"

#include "GL/glew.h"
#include "OpenGLErrorhandler.h"
#include "Core/Logger.h"
#include "RenderAPI/Buffers.h"

#include "Vender/glm/glm/glm.hpp"
#include <algorithm>

/*
* TODO Setup gpu side updater, so when there is a mesh update can send single mesh push
*/

namespace ME
{
    namespace OpenGL
    {
        OpenGLRendererAPI::OpenGLRendererAPI()
        :RenderAPI(RenderAPItype::ME_RENDERER_OPENGL), m_clearcolor(glm::vec4{0.0f, 0.0f, 0.0f, 0.0f}), vertex_id(0), index_id(0),
            m_TotalVertices(0), m_TotalIndices(0), m_NextIndex(0)
        {
            ME_PROFILE_TRACE_CALL();

            m_Layout = CreateRef<VertexbufferLayout>();
            m_Layout->push(GL_FLOAT, 3);
            m_Layout->push(GL_FLOAT, 2);
            m_Layout->push(GL_FLOAT, 1);
        }

        OpenGLRendererAPI::~OpenGLRendererAPI()
        {

            ME_PROFILE_TRACE_CALL();

        }

        void OpenGLRendererAPI::SetViewPortSize(const unsigned int& X, const unsigned int& Y)
        {
            glViewport(0, 0, X, Y);
        }

        void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
        {

            ME_PROFILE_TRACE_CALL();

            m_clearcolor = color;
        }

        void OpenGLRendererAPI::SetShader(const Ref<Shader>& shader)
        {

            ME_PROFILE_TRACE_CALL();

            m_Shader = shader;
        }

        void OpenGLRendererAPI::AddFramebuffer(const FramebufferSpecification& framebuffer)
        {
            m_FrameSpec = framebuffer;
        }

        void OpenGLRendererAPI::OnAttach()
        {

            ME_PROFILE_TRACE_CALL();
//
// Initiating graphics libraries
// Need to add graphics drivers identification
            ME_CORE_ERROR(glewInit() != GLEW_OK, "Can't Impliment GLEW")
#ifdef ME_DEBUG
            ME_CORE_INFO("Detected OpenGL Version (using) : {}", glGetString(GL_VERSION));
#endif
//
// Enabling blending, typically transparencies
//
            GLLogCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            GLLogCall(glEnable(GL_BLEND));

            GLLogCall(glGenBuffers(1, &vertex_id));
            GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_id));
            GLLogCall(glBufferData(GL_ARRAY_BUFFER, ME_OPENGL_MAXVERTEXBUFFER * sizeof(VERTEX), nullptr, GL_DYNAMIC_DRAW));

            GLLogCall(glGenBuffers(1, &index_id));
            GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id));
            GLLogCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, ME_OPENGL_MAXINDEXBUFFER * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW));

            GLLogCall(glBindVertexArray(1));

            auto& elements = m_Layout->GetElements();
            unsigned int offset = 0;

            for (unsigned int j = 0; j < elements.size(); j++)
            {
                const VertexbufferElement element = elements[j];
                GLLogCall(glEnableVertexAttribArray(j));
                GLLogCall(glVertexAttribPointer(j, element.count, element.type, element.normalized, m_Layout->GetStride(), (const void*)offset));
                offset += GetElementSize(element.type) * element.count;
            }
        }

        void OpenGLRendererAPI::OnUpdate(Timestep ts)
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_id));
            for (Mesh* ms : m_Update)
                glBufferSubData(GL_ARRAY_BUFFER, ms->GetMeshData().vertex.GetOffset() * sizeof(VERTEX),
                    ms->GetMeshData().vertex.Size() * sizeof(VERTEX),
                    ms->GetMeshData().vertex.begin());
            m_Update.clear();

            GLLogCall(glClearColor(m_clearcolor.x, m_clearcolor.y, m_clearcolor.z, m_clearcolor.w));
            GLLogCall(glClear(GL_COLOR_BUFFER_BIT));
        }

        void OpenGLRendererAPI::OnDetach()
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glDeleteBuffers(1, &vertex_id));
            GLLogCall(glDeleteBuffers(1, &index_id));
        }

        void OpenGLRendererAPI::OnDraw()
        {

            ME_PROFILE_TRACE_CALL();
//
// Will Soon Sort out the shaders when Materials Are Implemented
//
// Setting up VertexArray for each call, needs to be fixed at higher builds!!
//              
            GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_id));
            GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id));

            if(framebuffer.IsValid())
                framebuffer->Bind();

            m_Shader->Bind();

            GLLogCall(glDrawElements(GL_TRIANGLES, m_TotalIndices, GL_UNSIGNED_INT, nullptr));

            if (framebuffer.IsValid())
                framebuffer->unBind();
        }

        void OpenGLRendererAPI::OnEvent(Event::Event& e)
        {

            ME_PROFILE_TRACE_CALL();
//
// Logs out missed event handles
//
#ifdef ME_DEBUG_SHOW_EVENT
            ME_CORE_INFO(e.ToString());
#endif
        }

        void OpenGLRendererAPI::AddMesh(const Ref<Mesh2D>& mesh)
        {
            Ref<Mesh> ms = mesh;
            m_Mesh2D.emplace_back(ms);
        }            
        void OpenGLRendererAPI::AddMesh(const std::vector<Ref<Mesh2D>>& meshes)
        {
            for (Ref<Mesh2D> ms : meshes)
            {
                m_Mesh2D.emplace_back(ms);
            }
        }
        void OpenGLRendererAPI::AddMesh(const Ref<Mesh>& mesh)
        {
            Ref<Mesh> ms = mesh;
            m_Meshes.emplace_back(ms);
            ms->GetMeshData().vertex.SetOffset(m_TotalVertices);
            ms->GetMeshData().index.SetOffset(m_TotalIndices);
            ms->GetMeshData().index.SetIndexOffset(m_NextIndex);

            GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_id));
            GLLogCall(glBufferSubData(GL_ARRAY_BUFFER, m_TotalVertices * sizeof(VERTEX), 
                ms->GetMeshData().vertex.Size() * sizeof(VERTEX),
                ms->GetMeshData().vertex.begin()));

            GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id));
            unsigned int max = 0;
            unsigned int* local = alloc<unsigned int>(ms->GetMeshData().index.Size());
            for (unsigned int i = 0; i < ms->GetMeshData().index.Size(); i++)
            {
                unsigned int j = ms->GetMeshData().index.begin()[i];
                max < j ? max = j : max;
                local[i] = j + m_NextIndex;
            }

            GLLogCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_TotalIndices * sizeof(unsigned int), 
                    ms->GetMeshData().index.Size() * sizeof(unsigned int), local));

            dealloc(local);

            m_NextIndex += (max + 1);
            m_TotalVertices += ms->GetMeshData().vertex.Size();
            m_TotalIndices += ms->GetMeshData().index.Size();
        }
        void OpenGLRendererAPI::AddMesh(const std::vector<Ref<Mesh>>& meshes)
        {
            for (Ref<Mesh> ms : meshes)
            {
                m_Meshes.emplace_back(ms);
                ms->GetMeshData().vertex.SetOffset(m_TotalVertices);
                ms->GetMeshData().index.SetOffset(m_TotalIndices);
                ms->GetMeshData().index.SetIndexOffset(m_NextIndex);

                GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_id));
                GLLogCall(glBufferSubData(GL_ARRAY_BUFFER, m_TotalVertices * sizeof(VERTEX),
                    ms->GetMeshData().vertex.Size() * sizeof(VERTEX),
                    ms->GetMeshData().vertex.begin()));

                GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id));
                unsigned int max = 0;
                unsigned int* local = alloc<unsigned int>(ms->GetMeshData().index.Size());
                for (unsigned int i = 0; i < ms->GetMeshData().index.Size(); i++)
                {
                    unsigned int j = ms->GetMeshData().index.begin()[i];
                    max < j ? max = j : max;
                    local[i] = j + m_NextIndex;
                }

                GLLogCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_TotalIndices * sizeof(unsigned int),
                    ms->GetMeshData().index.Size() * sizeof(unsigned int), local));

                dealloc(local);

                m_NextIndex += (max + 1);
                m_TotalIndices += ms->GetMeshData().index.Size();
                m_TotalVertices += ms->GetMeshData().vertex.Size();
            }
        }

        void OpenGLRendererAPI::PushUpdate(Mesh* mesh)
        {
            m_Update.emplace_back(mesh);
        }

        void OpenGLRendererAPI::PushUpdate(Mesh2D* mesh)
        {
            m_Update2D.emplace_back(mesh);
        }
    }
}