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
            std::vector<Layer*> m_layerstack;
            unsigned int m_totallayers = 0;
        public:
            LayerStack() = default;
            ~LayerStack();

            void PushLayer(Layer* layer);
            void PushOverlay(Layer* overlay);
            void PopLayer(Layer* layer);
            void PopOverlay(Layer* overlay);

            std::vector<Layer*>::iterator begin() {return m_layerstack.begin();}
            std::vector<Layer*>::iterator end() {return m_layerstack.end();}
            std::vector<Layer*>::reverse_iterator rbegin() {return m_layerstack.rbegin();}
            std::vector<Layer*>::reverse_iterator rend() {return m_layerstack.rend();}

            std::vector<Layer*>::const_iterator begin() const {return m_layerstack.begin();}
            std::vector<Layer*>::const_iterator end() const {return m_layerstack.end();}
            std::vector<Layer*>::const_reverse_iterator rbegin() const {return m_layerstack.rbegin();}
            std::vector<Layer*>::const_reverse_iterator rend() const {return m_layerstack.rend();}
        };
    }
}