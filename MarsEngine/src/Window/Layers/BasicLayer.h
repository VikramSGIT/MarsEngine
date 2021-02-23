#pragma once
#include "MarsHeader.h"

#include "Layer.h"
#include <functional>
#include "Window/Events/Event.h"
namespace ME
{
    namespace Layer
    {
        class BasicLayer : public Window::Layer::Layer
        {
        private:
            void EmptyFunction() {}
            void EmptyOnEvent(Event::Event& e) {}

            std::function<void()> onattach = std::bind(&BasicLayer::EmptyFunction, this);
            std::function<void()> ondetach = std::bind(&BasicLayer::EmptyFunction, this);
            std::function<void()> onupdate = std::bind(&BasicLayer::EmptyFunction, this);
            std::function<void(Event::Event&)> onevent = std::bind(&BasicLayer::EmptyOnEvent, this, std::placeholders::_1);
        public:
            BasicLayer()
                :Layer("OpenGLRenderer") {}
            ~BasicLayer() = default;

            void SetOnAttach(std::function<void()> func);
            void SetOnDetach(std::function<void()> func);
            void SetOnUpdate(std::function<void()> func);
            void SetOnEvent(std::function<void(Event::Event&)> func);

            void OnAttach() override;
            void OnDetach() override;
            void OnUpdate() override;
            void OnEvent(Event::Event& e) override;
        };
    }
}