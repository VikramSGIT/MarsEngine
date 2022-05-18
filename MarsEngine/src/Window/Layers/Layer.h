#ifndef ME_LAYER
#define ME_LAYER

#pragma once

#include "Window/Events/Event.h"
#include "Core/Utilites/TimeStep.h"
#include "Core/Utilites/String.h"

namespace ME
{
    namespace Window
    {
        namespace Layer
        {
            class Layer
            {
            private:
                String<alloc_dealloc_UpstreamMemory> m_LayerName;
            public:
                Layer(const String<alloc_dealloc_UpstreamMemory>& name)
                    :m_LayerName(name) {}
                virtual ~Layer() = default;

                virtual void OnAttach() = 0;
                virtual void OnDetach() = 0;
                virtual void OnUpdate(Timestep ts) = 0;
                virtual void OnDraw() = 0;
                virtual void OnEvent(Event::Event& e) = 0;

                inline String<alloc_dealloc_UpstreamMemory> GetName() const { return m_LayerName; }
            };
        }
    }
}

#endif