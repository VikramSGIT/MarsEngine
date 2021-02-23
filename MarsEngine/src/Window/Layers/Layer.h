#pragma once
#include "MarsHeader.h"

#include "Window/Events/Event.h"

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
                virtual void OnUpdate() = 0;
                virtual void OnEvent(Event::Event& e) = 0;

                const std::string GetName() const { return m_LayerName; }
            };
        }
    }
}