#pragma once

#include <spatial/core/Application.h>

#include <spatial/desktop/PlatformEvent.h>

#include <spatial/render/CommonResources.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/UserInterfaceRenderer.h>

namespace spatial::render
{

class UserInterfaceSystem
{
private:
    UserInterfaceRenderer m_ui;

    const desktop::Window *m_window;

    void setupViewport();

public:
    UserInterfaceSystem(core::Application& app, RenderingSystem& rendering);

    void onStart();

    void onStartFrame(float delta);

    void onUpdateFrame(float delta);

    void onEvent(const desktop::WindowResizedEvent &event);
};

} // namespace spatial::render