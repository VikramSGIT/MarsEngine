#ifndef ME_LAYERSTACK
#define ME_LAYERSTACK

#pragma once

#include "Layer.h"
#include "Core/Utilites/Vector.h"
#include "Core/Utilites/Ref.h"
namespace ME
{
    namespace Window
    {
        class LayerStack
        {
        private:
            Vector<Ref<Layer>> m_layerstack;
        public:
            LayerStack() = default;
            ~LayerStack();

            void PushOverlay(const Ref<Layer>& overlay);
            void PushLayer(const Ref<Layer>& layer);
            void PopLayer(const Ref<Layer>& layer);

            void clear() { m_layerstack.clear(); }
            auto begin() { return m_layerstack.begin(); }
            auto end() { return m_layerstack.end(); }

            auto begin() const { return m_layerstack.begin(); }
            auto end() const { return m_layerstack.end(); }

            inline size_t GetTotalLayers() const { return m_layerstack.size(); }
        };
    }
}

#endif