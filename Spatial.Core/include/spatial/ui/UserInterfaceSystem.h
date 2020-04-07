#pragma once

#include <spatial/common/EventQueue.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/render/RenderingSystem.h>

#include <spatial/ui/UserInterfaceRenderer.h>
#include <spatial/ui/UserInterfaceInput.h>

#include <filesystem>

namespace spatial
{

class UserInterfaceSystem
{
private:
	UserInterfaceRenderer m_renderer;
	UserInterfaceInput m_input{};

	const Window* m_window;

	std::filesystem::path m_fontPath;

	void setupViewport();

public:
	UserInterfaceSystem(RenderingSystem& rendering, const std::filesystem::path& fontPath);

	void attach(EventQueue& queue);
	void detach(EventQueue& queue);

	void onStart();

	void onStartFrame(float delta);

	void onUpdateGuiFrame(float delta);

	void onEvent(const WindowResizedEvent& event);

	void onEvent(const MouseMovedEvent& event);

	void onEvent(const KeyEvent& event);

	void onEvent(const TextEvent& event);
};

} // namespace spatial::ui