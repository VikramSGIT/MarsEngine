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
                {
#ifdef ME_DEBUG
                    ss << "Detected OpenGL Vesrion (using) : " << glGetString(GL_VERSION);
                    ME_CORE_INFO(ss.str());
#endif
                }
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
                for (unsigned int i : vertexbuffercache)
                    OpenGLVertexBuffer vb(i);
                vertexbuffercache.clear();
                for (unsigned int i : indexbuffercache)
                    OpenGLIndexBuffer ib(i);
                indexbuffercache.clear();
            }

            void OpenGLRendererAPI::AddRenderSubmition(const MeshQueue& meshqueue, std::function<void()> preprocessdata)
            {
                m_RenderQueue.push_back(meshqueue);
                preprocessing.emplace_back(preprocessdata);
            }

            void OpenGLRendererAPI::SetUpBuffers(const MeshQueue& meshqueue)
            {
                Ref<VertexBuffer> vertexbufferobj = CreateRef<OpenGLVertexBuffer>(ME_MAX_VERTEX_BUFFER_SIZE * sizeof(ME_DATATYPE), GL_DYNAMIC_DRAW);
                Ref<IndexBuffer> indexbufferobj = CreateRef<OpenGLIndexBuffer>(ME_MAX_INDEX_BUFFER_SIZE * sizeof(unsigned int), GL_DYNAMIC_DRAW);

                vertexbufferobj->BufferPostRenderData(meshqueue.GetVertexBuffer(), meshqueue.GetTotalVertices(), 0);
                indexbufferobj->BufferPostRenderData(meshqueue.GetIndexBuffer(), meshqueue.GetTotalIndices(), 0);

                vertexbufferobj->ClearBufferOnDestroy(false);
                indexbufferobj->ClearBufferOnDestroy(false);

                vertexbuffercache.push_back(vertexbufferobj->GetID());
                indexbuffercache.push_back(indexbufferobj->GetID());
            }

            void OpenGLRendererAPI::CheckBufferUpdate(const unsigned int& id)
            {

                ME_PROFILE_TRACE_CALL();

                for (glm::uvec2 range : m_RenderQueue[id].GetUpdate())
                {
                    Ref<VertexBuffer> vb = CreateRef<OpenGLVertexBuffer>(vertexbuffercache[id]);
                    vb->ClearBufferOnDestroy(false);
                    vb->BufferPostRenderData((m_RenderQueue[id].GetVertexBuffer() + range.x), (range.y - range.x), range.x);
                }
            }

            void OpenGLRendererAPI::Draw(const Ref<Shader>& shader)
            {

                ME_PROFILE_TRACE_CALL();
                //
                // Will Soon Sort out the shaders when Materials Are Implemented
                //

                for (unsigned __int64 i = 0; i < m_RenderQueue.size(); i++)
                {
                    //
                    // Constructing buffers for each meshqueue
                    //
                    if (!Ready)
                    {
                        if ((i + 1) == m_RenderQueue.size())
                            Ready = true;
                        SetUpBuffers(m_RenderQueue[i]);
                    }
                    //
                    // Checks for buffer Update
                    //
                    CheckBufferUpdate(i);
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

                    shader->Bind();
                    indexbuffer->Bind();
                    vertexbuffer->Bind();

                    GLLogCall(glDrawElements(GL_TRIANGLES, m_RenderQueue[i].GetTotalIndices(), GL_UNSIGNED_INT, nullptr));

                    vertexbuffer->unBind();
                    indexbuffer->unBind();
                    shader->unBind();
                }
            }

            bool OpenGLRendererAPI::SwitchAPI(const RenderAPItype api)
            {

                ME_PROFILE_TRACE_CALL();

                return true;
            }

            Ref<Layer::BasicLayer> OpenGLRendererAPI::GetLayer()
            {

                ME_PROFILE_TRACE_CALL();

                Ref<Layer::BasicLayer> layer = CreateRef<Layer::BasicLayer>();
                layer->SetOnUpdate(std::bind(&OpenGLRendererAPI::OnUpdate, this));
                layer->SetOnEvent(std::bind(&OpenGLRendererAPI::OnEvent, this, std::placeholders::_1));
                return layer;
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