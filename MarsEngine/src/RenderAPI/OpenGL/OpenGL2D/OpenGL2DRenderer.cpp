#include "OpenGL2DRenderer.h"
#include "Core/Memory/SafePointer.h"

/*
* TODO Setup gpu side updater, so when there is a mesh update can send single mesh push
*/

namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            OpenGL2DRendererAPI::OpenGL2DRendererAPI()
            :Render2DAPI(Render2DAPItype::ME_RENDERER2D_OPENGL)
            {

                ME_PROFILE_TRACE_CALL();
            }

            OpenGL2DRendererAPI::~OpenGL2DRendererAPI()
            {

                ME_PROFILE_TRACE_CALL();

                ClearBufferCache();
            }

            void OpenGL2DRendererAPI::SetViewPortSize(const unsigned int& X, const unsigned int& Y)
            {
                glViewport(0, 0, X, Y);
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

            void OpenGL2DRendererAPI::Init()
            {

                ME_PROFILE_TRACE_CALL();
//
// Initiating graphics libraries
// Need to add graphics drivers identification
                ME_CORE_CRITICAL(glewInit() != GLEW_OK, "Can't Impliment GLEW")
#ifdef ME_DEBUG
                std::stringstream ss;
                ss << "Detected OpenGL Version (using) : " << glGetString(GL_VERSION);
                ME_CORE_INFO(ss.str());
#endif
//
// Enabling blending, typically transparencies
//
                GLLogCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
                GLLogCall(glEnable(GL_BLEND));
            }

            void OpenGL2DRendererAPI::OnUpdate()
            {

                ME_PROFILE_TRACE_CALL();

                Clear();
            }

            void OpenGL2DRendererAPI::Clear() const
            {

                ME_PROFILE_TRACE_CALL();

                GLLogCall(glClearColor(m_clearcolor.x, m_clearcolor.y, m_clearcolor.z, m_clearcolor.w));
                GLLogCall(glClear(GL_COLOR_BUFFER_BIT));
            }

            void OpenGL2DRendererAPI::ClearBufferCache()
            {

                ME_PROFILE_TRACE_CALL();

                for (unsigned int i : vertexbuffercache)
                    OpenGLVertexBuffer vb(i);
                vertexbuffercache.clear();
                for (unsigned int i : indexbuffercache)
                    OpenGLIndexBuffer ib(i);
                indexbuffercache.clear();
            }

            void OpenGL2DRendererAPI::AddRenderSubmition(const Ref<MeshQueue2D>& meshqueue, std::function<void()> preprocessdata)
            {

                ME_PROFILE_TRACE_CALL();

                m_RenderQueue.emplace_back(meshqueue);
                preprocessing.emplace_back(preprocessdata);
                SetUpBuffers(meshqueue);
            }

            void OpenGL2DRendererAPI::SetUpBuffers(const Ref<MeshQueue2D>& meshqueue)
            {

                ME_PROFILE_TRACE_CALL();
                    
                Ref<ME::Renderer::VertexBufferLayout> layout = meshqueue->GetLayout();
                unsigned int voffset = 0, ioffset = 0, indexoffset = 0;
                
                SafePointer<VERTEX2D> vertexbuffer(allocarr<VERTEX2D>(meshqueue->GetTotalVertices()), meshqueue->GetTotalVertices());
                SafePointer<unsigned int> indexbuffer(allocarr<unsigned int>(meshqueue->GetTotalIndices()), meshqueue->GetTotalIndices());

                for (Ref<Mesh2D> ms : *meshqueue)
                {
                    memcpy(vertexbuffer + voffset, ms->GetMeshData().vertex.begin(), sizeof(VERTEX2D) * ms->GetMeshData().vertex.Size());
                    ms->GetMeshData().vertex.SetOffset(voffset);
                    voffset += ms->GetMeshData().vertex.Size();

                    ms->GetMeshData().index.SetOffset(ioffset);
                    for (unsigned int i = 0; i < ms->GetMeshData().index.Size(); i++)
                        indexbuffer[i + ioffset] = ms->GetMeshData().index.begin()[i] + indexoffset;

                    ioffset += ms->GetMeshData().index.Size();
                    indexoffset += 1 + *std::max_element(ms->GetMeshData().index.begin(), ms->GetMeshData().index.end());
                }

                Ref<VertexBuffer> vertexbufferobj = CreateRef<OpenGLVertexBuffer>((ME_DATATYPE*)vertexbuffer, meshqueue->GetTotalVertices(), GL_DYNAMIC_DRAW);
                Ref<IndexBuffer> indexbufferobj = CreateRef<OpenGLIndexBuffer>(indexbuffer, meshqueue->GetTotalIndices(), GL_DYNAMIC_DRAW);

                dealloc(vertexbuffer.Get(), meshqueue->GetTotalVertices() * sizeof(VERTEX2D));
                dealloc(indexbuffer.Get(), meshqueue->GetTotalIndices() * sizeof(unsigned int));
                
                vertexbufferobj->ClearBufferOnDestroy(false);
                indexbufferobj->ClearBufferOnDestroy(false);

                vertexbuffercache.push_back(vertexbufferobj->GetID());
                indexbuffercache.push_back(indexbufferobj->GetID());
            }

            void OpenGL2DRendererAPI::CheckBufferUpdate(const unsigned int& id)
            {
                ME_PROFILE_TRACE_CALL();

                for (std::pair<Mesh2D*, unsigned int> data : m_RenderQueue[id]->GetUpdate())
                {
                    Ref<VertexBuffer> vertexbuffer = CreateRef<OpenGLVertexBuffer>(vertexbuffercache[id]);
                    vertexbuffer->BufferPostRenderData(data.first->GetMeshData().vertex.begin(), 
                        m_RenderQueue[id]->GetLayout()->GetTotalCount() * data.first->GetMeshData().vertex.Size(), 
                        m_RenderQueue[id]->GetLayout()->GetTotalCount() * data.second);
                    vertexbuffer->ClearBufferOnDestroy(false);
                    data.first->SetReady(true);
                }
            }

            void OpenGL2DRendererAPI::OnDraw()
            {

                ME_PROFILE_TRACE_CALL();
//
// Will Soon Sort out the shaders when Materials Are Implemented
//

                for (size_t i = 0; i < m_RenderQueue.size(); i++)
                {
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
                    Ref<OpenGLVertexArray> array = CreateRef<OpenGLVertexArray>();

                    vertexbuffer->ClearBufferOnDestroy(false);
                    indexbuffer->ClearBufferOnDestroy(false);

                    array->AddBuffer(*vertexbuffer, *m_RenderQueue[i]->GetLayout());

                    m_Shader->Bind();
                    indexbuffer->Bind();
                    vertexbuffer->Bind();

                    GLLogCall(glDrawElements(GL_TRIANGLES, m_RenderQueue[i]->GetTotalIndices(), GL_UNSIGNED_INT, nullptr));

                    vertexbuffer->unBind();
                    indexbuffer->unBind();
                    m_Shader->unBind();
                }
            }

            bool OpenGL2DRendererAPI::SwitchAPI(const Render2DAPItype api)
            {

                ME_PROFILE_TRACE_CALL();

                return true;
            }

            void OpenGL2DRendererAPI::OnEvent(Event::Event& e)
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