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

	std::filesystem::path m_fontPath;

	void setupViewport(const std::pair<int, int>& windowSize, const std::pair<int, int>& frameBufferSize);

public:
	UserInterfaceSystem(RenderingSystem& rendering, const Window& window, std::filesystem::path fontPath);

	void onStart();

	void onStartFrame(float delta);

	void onEndGuiFrame();

	void onEvent(const WindowResizedEvent& event);

	void onEvent(const MouseMovedEvent& event);

	void onEvent(const KeyEvent& event);

	void onEvent(const TextEvent& event);

	auto getView()
	{
		return m_renderer.getView();
	}
};
} // namespace spatial
