#pragma once
#include "MarsHeader.h"

#include "Layer.h"
#include "Window/Events/Event.h"

#include <functional>
namespace ME
{
    namespace Layer
    {
        class EmptyLayer : public Window::Layer::Layer
        {
        private:
            void EmptyFunction() {}
            void EmptyOnEvent(Event::Event& e) {}

            std::function<void()> onattach = std::bind(&EmptyLayer::EmptyFunction, this);
            std::function<void()> ondetach = std::bind(&EmptyLayer::EmptyFunction, this);
            std::function<void()> onupdate = std::bind(&EmptyLayer::EmptyFunction, this);
            std::function<void(Event::Event&)> onevent = std::bind(&EmptyLayer::EmptyOnEvent, this, std::placeholders::_1);
        public:
            EmptyLayer()
                :Layer("EmptyLayer") {}
            ~EmptyLayer() = default;

            void SetOnAttach(std::function<void()> func);
            void SetOnDetach(std::function<void()> func);
            void SetOnUpdate(std::function<void()> func);
            void SetOnEvent(std::function<void(Event::Event&)> func);

            void OnAttach() override;
            void OnDetach() override;
            void OnUpdate(Timestep ts) override;
            void OnEvent(Event::Event& e) override;
        };
    }
}