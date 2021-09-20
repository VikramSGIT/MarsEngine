#include "OpenGLRenderer.h"

namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            OpenGLRendererAPI::OpenGLRendererAPI()
                :RenderAPI(RenderAPItype::ME_RENDERER_OPENGL)
            {

                ME_PROFILE_TRACE_CALL();

            }

            OpenGLRendererAPI::~OpenGLRendererAPI()
            {

                ME_PROFILE_TRACE_CALL();

                ClearBufferCache();
                for (MeshQueue mq : m_RenderQueue)
                    mq.ClearBuffer();
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

            void OpenGLRendererAPI::Init()
            {

                ME_PROFILE_TRACE_CALL();
//
// Initiating graphics libraries
// Need to add graphics drivers identification
                std::stringstream ss;
                if (glewInit() != GLEW_OK)
                    ME_CORE_ERROR("Can't Impliment GLEW");
                else
#ifdef ME_DEBUG
                    ss << "Detected OpenGL Vesrion (using) : " << glGetString(GL_VERSION);
                ME_CORE_INFO(ss.str());
#endif
//
// Enabling blending, typically transparencies
//
                GLLogCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
                GLLogCall(glEnable(GL_BLEND));
            }

            void OpenGLRendererAPI::OnUpdate()
            {

                ME_PROFILE_TRACE_CALL();

                Clear();
            }

            void OpenGLRendererAPI::Clear() const
            {

                ME_PROFILE_TRACE_CALL();

                GLLogCall(glClearColor(m_clearcolor.x, m_clearcolor.y, m_clearcolor.z, m_clearcolor.w));
                GLLogCall(glClear(GL_COLOR_BUFFER_BIT));
            }

            void OpenGLRendererAPI::ClearBufferCache()
            {

                ME_PROFILE_TRACE_CALL();

                for (unsigned int i : vertexbuffercache)
                    OpenGLVertexBuffer vb(i);
                vertexbuffercache.clear();
                for (unsigned int i : indexbuffercache)
                    OpenGLIndexBuffer ib(i);
                indexbuffercache.clear();
            }

            void OpenGLRendererAPI::AddRenderSubmition(const MeshQueue& meshqueue, std::function<void()> preprocessdata)
            {

                ME_PROFILE_TRACE_CALL();

                m_RenderQueue.push_back(meshqueue);
                preprocessing.emplace_back(preprocessdata);
            }

            void OpenGLRendererAPI::SetUpBuffers(MeshQueue& meshqueue)
            {

                ME_PROFILE_TRACE_CALL();

                if (!meshqueue.IsReady())
                {
                    Ref<VertexBuffer> vertexbufferobj = CreateRef<OpenGLVertexBuffer>(ME_MAX_VERTEX_BUFFER_SIZE * sizeof(ME_DATATYPE), GL_DYNAMIC_DRAW);
                    Ref<IndexBuffer> indexbufferobj = CreateRef<OpenGLIndexBuffer>(ME_MAX_INDEX_BUFFER_SIZE * sizeof(unsigned int), GL_DYNAMIC_DRAW);
                    if (meshqueue.GetAllocationMode() == ALLOCMODE::ALLATONE)
                    {
                        vertexbufferobj->BufferPostRenderData(meshqueue.GetVertexBuffer(), meshqueue.GetTotalVertices(), 0);
                        indexbufferobj->BufferPostRenderData(meshqueue.GetIndexBuffer(), meshqueue.GetTotalIndices(), 0);
                    }
                    else if (meshqueue.GetAllocationMode() == ALLOCMODE::DISTRIBUTED)
                    {
                        std::allocator<unsigned int> iallocator;
                        Ref<ME::Renderer::VertexBufferLayout> layout = meshqueue.GetLayout();
                        unsigned int voffset = 0u, ioffset = 0u, indexoffset = 0u;

                        for (Ref<Mesh> ms : meshqueue)
                        {
                            std::pair<ME_DATATYPE const*, unsigned int> vertex = ms->GetVertexData();
                            vertexbufferobj->BufferPostRenderData(vertex.first, vertex.second * layout->GetTotalCount(), voffset);
                            voffset += vertex.second * layout->GetTotalCount();

                            std::pair<unsigned int const*, unsigned int> index = ms->GetIndexData();
                            unsigned int* indexbuffer = iallocator.allocate(index.second);
                            for (size_t i = 0; i < index.second; i++)
                                indexbuffer[i] = index.first[i] + indexoffset;

                            indexbufferobj->BufferPostRenderData(indexbuffer, index.second, ioffset);
                            iallocator.deallocate(indexbuffer, index.second);

                            ioffset += index.second;
                            indexoffset += 1 + *std::max_element(index.first, index.first + index.second);
                            ms->SetReady(true);
                        }
                    }
                    vertexbufferobj->ClearBufferOnDestroy(false);
                    indexbufferobj->ClearBufferOnDestroy(false);

                    vertexbuffercache.push_back(vertexbufferobj->GetID());
                    indexbuffercache.push_back(indexbufferobj->GetID());
                    meshqueue.SetReady(true);
                }
            }

            void OpenGLRendererAPI::CheckBufferUpdate(const unsigned int& id)
            {

                ME_PROFILE_TRACE_CALL();
                if (m_RenderQueue[id].GetAllocationMode() == ALLOCMODE::ALLATONE)
                {
                    for (glm::vec<2, unsigned int> ranges : m_RenderQueue[id].GetUpdate())
                    {
                        Ref<VertexBuffer> vb = CreateRef<OpenGLVertexBuffer>(vertexbuffercache[id]);
                        vb->ClearBufferOnDestroy(false);
                        vb->BufferPostRenderData((m_RenderQueue[id].GetVertexBuffer() + ranges.x), (ranges.y - ranges.x), ranges.x);
                    }
                }
                else if(m_RenderQueue[id].GetAllocationMode() == ALLOCMODE::DISTRIBUTED)
                    for (glm::vec<2, unsigned int> ranges : m_RenderQueue[id].GetUpdate())
                    {
                        std::vector<Ref<Mesh>> meshes = m_RenderQueue[id].GetMeshes();
                        Ref<VertexBuffer> vb = CreateRef<OpenGLVertexBuffer>(vertexbuffercache[id]);
                        vb->ClearBufferOnDestroy(false);
                        std::pair<float const*, unsigned int> vertex = m_RenderQueue[id].GetMeshes().at(ranges.y)->GetVertexData();
                        vb->BufferPostRenderData(vertex.first, vertex.second * m_RenderQueue[id].GetLayout()->GetTotalCount(), ranges.x);
                    }
            }

            void OpenGLRendererAPI::OnDraw()
            {

                ME_PROFILE_TRACE_CALL();
//
// Will Soon Sort out the shaders when Materials Are Implemented
//

                for (size_t i = 0; i < m_RenderQueue.size(); i++)
                {
//
// Constructing buffers for each meshqueue
//
                    
                    SetUpBuffers(m_RenderQueue[i]);
//
// Checks for buffer Update
//
                    CheckBufferUpdate(static_cast<unsigned int>(i));
//
// Setting up VertexArray for each call, needs to be fixed at higher builds!!
//              
                    preprocessing.at(i)();

                    Ref<VertexBuffer> vertexbuffer = CreateRef<OpenGLVertexBuffer>(vertexbuffercache[i]);
                    Ref<IndexBuffer> indexbuffer = CreateRef<OpenGLIndexBuffer>(indexbuffercache[i]);
                    Ref<VertexArray> array = CreateRef<OpenGLVertexArray>();

                    vertexbuffer->ClearBufferOnDestroy(false);
                    indexbuffer->ClearBufferOnDestroy(false);

                    array->AddBuffer(*vertexbuffer, *m_RenderQueue[i].GetLayout());

                    m_Shader->Bind();
                    indexbuffer->Bind();
                    vertexbuffer->Bind();

                    GLLogCall(glDrawElements(GL_TRIANGLES, m_RenderQueue[i].GetTotalIndices(), GL_UNSIGNED_INT, nullptr));

                    vertexbuffer->unBind();
                    indexbuffer->unBind();
                    m_Shader->unBind();
                }
            }

            bool OpenGLRendererAPI::SwitchAPI(const RenderAPItype api)
            {

                ME_PROFILE_TRACE_CALL();

                return true;
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
        }
    }
}