#include "MarsHeader.h"
#include "OpenGL2DRenderer.h"

#include "Core/Logger.h"
#include "RenderAPI/OpenGL/OpenGLErrorhandler.h"
#include "RenderAPI/OpenGL/OpenGLShader.h"

#include "GL/glew.h"
#include <algorithm>

namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            OpenGL2DRendererAPI::OpenGL2DRendererAPI()
                :RenderAPI(RenderAPItype::ME_RENDERER_OPENGL2D), m_clearcolor(glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f }),
                    vertex_id(0), index_id(0), m_TotalVertices(0), m_TotalIndices(0), m_NextIndex(0)
            {

                ME_PROFILE_TRACE_CALL();

                m_Layout = CreateRef<VertexbufferLayout>();
                m_Layout->push(GL_FLOAT, 2);
                m_Layout->push(GL_FLOAT, 2);
                m_Layout->push(GL_FLOAT, 1);
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
                m_Update.emplace_back(mesh);
            }

            void OpenGL2DRendererAPI::SetClearColor(const glm::vec4& color)
            {

                ME_PROFILE_TRACE_CALL();

                m_clearcolor = color;
            }

            void OpenGL2DRendererAPI::SetShader(const Ref<Shader>& shader)
            {

                ME_PROFILE_TRACE_CALL();

                m_Shader = shader;
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

                GLLogCall(glGenBuffers(1, &vertex_id));
                GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_id));
                GLLogCall(glBufferData(GL_ARRAY_BUFFER, ME_OPENGL_MAXVERTEXBUFFER * sizeof(VERTEX2D), nullptr, GL_DYNAMIC_DRAW));

                GLLogCall(glGenBuffers(1, &index_id));
                GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id));
                GLLogCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, ME_OPENGL_MAXINDEXBUFFER * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW));

                GLLogCall(glBindVertexArray(1));

                unsigned int offset = 0;

                auto& elements = m_Layout->GetElements();

                for (unsigned int j = 0; j < elements.size(); j++)
                {
                    auto element = elements[j];
                    GLLogCall(glEnableVertexAttribArray(j));
                    GLLogCall(glVertexAttribPointer(j, element.count, element.type, element.normalized, m_Layout->GetStride(), (const void*)offset));
                    offset += GetElementSize(element.type) * element.count;
                }
            }

            void OpenGL2DRendererAPI::OnDetach()
            {
                GLLogCall(glDeleteBuffers(1, &vertex_id));
                GLLogCall(glDeleteBuffers(1, &index_id));
            }

            void OpenGL2DRendererAPI::OnUpdate(Timestep ts)
            {

                ME_PROFILE_TRACE_CALL();

                GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_id));
                for (auto ms : m_Update)
                {
                    GLLogCall(glBufferSubData(GL_ARRAY_BUFFER, ms->GetMeshData().vertex.GetOffset() * sizeof(VERTEX2D),
                        ms->GetMeshData().vertex.Size() * sizeof(VERTEX2D),
                        ms->GetMeshData().vertex.begin()));
                }
                m_Update.clear();

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
                GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_id));
                GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id));

                //framebuffer->Bind();
                m_Shader->Bind();

                GLLogCall(glDrawElements(GL_TRIANGLES, m_TotalIndices, GL_UNSIGNED_INT, nullptr));
            }

            void OpenGL2DRendererAPI::OnEvent(Event::Event& e)
            {

                ME_PROFILE_TRACE_CALL();

            }
            void OpenGL2DRendererAPI::AddMesh(const Ref<Mesh2D>& mesh)
            {
                Ref<Mesh2D> ms = mesh;
                m_Meshes.emplace_back(ms);
                ms->GetMeshData().vertex.SetOffset(m_TotalVertices);
                ms->GetMeshData().index.SetOffset(m_TotalIndices);
                ms->GetMeshData().index.SetIndexOffset(m_NextIndex);

                GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_id));
                GLLogCall(glBufferSubData(GL_ARRAY_BUFFER, m_TotalVertices * sizeof(VERTEX2D),
                    ms->GetMeshData().vertex.Size() * sizeof(VERTEX2D),
                    ms->GetMeshData().vertex.begin()));

                unsigned int max = 0;
                unsigned int* local = alloc<unsigned int>(ms->GetMeshData().index.Size());
                for (unsigned int i = 0; i < ms->GetMeshData().index.Size(); i++)
                {
                    unsigned int j = ms->GetMeshData().index.begin()[i];
                    max < j ? max = j : max;
                    local[i] = j + m_NextIndex;
                }

                GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id));
                GLLogCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_TotalIndices * sizeof(unsigned int),
                    ms->GetMeshData().index.Size() * sizeof(unsigned int), local));

                dealloc(local);

                m_NextIndex += (max + 1);
                m_TotalVertices += ms->GetMeshData().vertex.Size();
                m_TotalIndices += ms->GetMeshData().index.Size();
            }
            void OpenGL2DRendererAPI::AddMesh(const std::vector<Ref<Mesh2D>>& meshes)
            {
                for (Ref<Mesh2D> ms : meshes)
                {
                    m_Meshes.emplace_back(ms);
                    ms->GetMeshData().vertex.SetOffset(m_TotalVertices);
                    ms->GetMeshData().index.SetOffset(m_TotalIndices);
                    ms->GetMeshData().index.SetIndexOffset(m_NextIndex);

                    GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_id));
                    GLLogCall(glBufferSubData(GL_ARRAY_BUFFER, m_TotalVertices * sizeof(VERTEX2D),
                        ms->GetMeshData().vertex.Size() * sizeof(VERTEX2D),
                        ms->GetMeshData().vertex.begin()));

                    unsigned int max = 0;
                    unsigned int* local = alloc<unsigned int>(ms->GetMeshData().index.Size());
                    for (unsigned int i = 0; i < ms->GetMeshData().index.Size(); i++)
                    {
                        unsigned int j = ms->GetMeshData().index.begin()[i];
                        max < j ? max = j : max;
                        local[i] = j + m_NextIndex;
                    }

                    GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id));
                    GLLogCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_TotalIndices * sizeof(unsigned int),
                        ms->GetMeshData().index.Size() * sizeof(unsigned int), local));

                    dealloc(local);

                    m_NextIndex += (max + 1);
                    m_TotalVertices += ms->GetMeshData().vertex.Size();
                    m_TotalIndices += ms->GetMeshData().index.Size();
                }
            }
        }
    }
}