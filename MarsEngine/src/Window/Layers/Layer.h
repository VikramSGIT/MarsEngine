#ifndef ME_LAYER
#define ME_LAYER

#include "MarsHeader.h"

#include "Window/Events/Event.h"
#include "Core/Utilites/TimeStep.h"

#include <string>
namespace ME
{
    namespace Window
    {
        namespace Layer
        {
            class Layer
            {
            private:
                std::string m_LayerName;
            public:
                Layer(const std::string& name)
                    :m_LayerName(name) {}
                virtual ~Layer() = default;

                virtual void OnAttach() = 0;
                virtual void OnDetach() = 0;
                virtual void OnUpdate(Timestep ts) = 0;
                virtual void OnDraw() = 0;
                virtual void OnEvent(Event::Event& e) = 0;

                inline std::string GetName() const { return m_LayerName; }
            };
        }
    }
}

#endif