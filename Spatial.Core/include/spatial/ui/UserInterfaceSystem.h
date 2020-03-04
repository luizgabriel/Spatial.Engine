#pragma once

#include <spatial/common/EventQueue.h>
#include <spatial/desktop/PlatformEvent.h>

#include <spatial/render/CommonResources.h>
#include <spatial/render/RenderingSystem.h>

#include <spatial/ui/UserInterfaceRenderer.h>
#include <spatial/ui/UserInterfaceInput.h>

#include <filesystem>

namespace spatial::ui
{

class UserInterfaceSystem
{
private:
    UserInterfaceRenderer m_renderer;
    UserInterfaceInput m_input;

    const desktop::Window *m_window;

    std::filesystem::path m_fontPath;

    void setupViewport();

public:
    UserInterfaceSystem(render::RenderingSystem &rendering, const std::filesystem::path &fontPath);

    void attach(common::EventQueue &queue);
    void detach(common::EventQueue &queue);

    void onStart();

    void onStartFrame(float delta);

    void onUpdateGuiFrame(float delta);

    void onEvent(const desktop::WindowResizedEvent &event);

    void onEvent(const desktop::MouseMovedEvent &event);

    void onEvent(const desktop::KeyEvent &event);

    void onEvent(const desktop::TextEvent &event);
};

} // namespace spatial::ui