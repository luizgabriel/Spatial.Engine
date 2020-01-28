#pragma once

#include <spatial/core/Application.h>
#include <spatial/desktop/PlatformEvent.h>

#include <spatial/render/CommonResources.h>
#include <spatial/render/RenderingSystem.h>

#include <spatial/ui/UserInterfaceRenderer.h>
#include <spatial/ui/UserInterfaceInput.h>

namespace spatial::ui
{

class UserInterfaceSystem
{
private:
    UserInterfaceRenderer m_renderer;
    UserInterfaceInput m_input;

    const desktop::Window *m_window;

    void setupViewport();

public:
    UserInterfaceSystem(core::Application& app, render::RenderingSystem& rendering);

    void onStart();

    void onStartFrame(float delta);

    void onUpdateFrame(float delta);

    void onEvent(const desktop::WindowResizedEvent &event);

    void onEvent(const desktop::MouseMovedEvent &event);

    void onEvent(const desktop::KeyEvent &event);

    void onEvent(const desktop::TextEvent &event);
};

} // namespace spatial::render