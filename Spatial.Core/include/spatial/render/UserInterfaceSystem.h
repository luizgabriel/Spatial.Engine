#pragma once

#include <spatial/render/CommonResources.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/core/ApplicationConnector.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/UserInterfaceRenderer.h>

namespace spatial::render
{

class UserInterfaceSystem
{
    using self_t = UserInterfaceSystem;

private:
    core::SignalsConnector<self_t> m_signalsConnector;
    core::EventConnector<desktop::WindowResizedEvent, self_t> m_windowResizedEventConnector;
    desktop::Window* m_window;
    UserInterfaceRenderer m_ui;

    void setupViewport();

public:
    UserInterfaceSystem(core::Application *app, RenderingSystem* rendering);

    void onStart();

    void onStartFrame(float delta);

    void onUpdateFrame(float delta);

    void onEvent(const desktop::WindowResizedEvent &event);
};

} // namespace spatial::render