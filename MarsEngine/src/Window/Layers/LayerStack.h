#pragma once
#include "MarsHeader.h"

#include "Layer.h"

#include <vector>
#include <algorithm>
#include <functional>

namespace Window
{
    namespace Layer
    {
        class LayerStack
        {
        private:
            std::vector<Ref<Layer>> m_layerstack;
            unsigned int m_totallayers = 0;
        public:
            LayerStack() = default;
            ~LayerStack();

            void PushLayer(Ref<Layer> layer);
            void PushOverlay(Ref<Layer> overlay);
            void PopLayer(Ref<Layer> layer);
            void PopOverlay(Ref<Layer> overlay);

            std::vector<Ref<Layer>>::iterator begin() {return m_layerstack.begin();}
            std::vector<Ref<Layer>>::iterator end() {return m_layerstack.end();}
            std::vector<Ref<Layer>>::reverse_iterator rbegin() {return m_layerstack.rbegin();}
            std::vector<Ref<Layer>>::reverse_iterator rend() {return m_layerstack.rend();}

            std::vector<Ref<Layer>>::const_iterator begin() const {return m_layerstack.begin();}
            std::vector<Ref<Layer>>::const_iterator end() const {return m_layerstack.end();}
            std::vector<Ref<Layer>>::const_reverse_iterator rbegin() const {return m_layerstack.rbegin();}
            std::vector<Ref<Layer>>::const_reverse_iterator rend() const {return m_layerstack.rend();}
        };
    }
}