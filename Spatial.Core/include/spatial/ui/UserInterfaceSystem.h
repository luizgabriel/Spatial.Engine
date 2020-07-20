#pragma once

#include <spatial/common/EventQueue.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/render/RenderingSystem.h>

#include <spatial/ui/UserInterfaceRenderer.h>
#include <spatial/ui/UserInterfaceInput.h>

#include <span>
#include <string_view>
#include <utility>

namespace spatial
{
class UserInterfaceSystem
{
private:
	UserInterfaceRenderer mRenderer;
	UserInterfaceInput mInput;

public:
	UserInterfaceSystem(filament::Engine& engine);

	UserInterfaceSystem(RenderingSystem& rendering);

	UserInterfaceSystem(RenderingSystem& rendering, const Window& window);

	void setDefaultFont(const std::string_view fontData);

	void setDefaultMaterial(const std::string_view materialData);

	void setViewport(const std::pair<int, int>& windowSize, const std::pair<int, int>& frameBufferSize);

	void onStart();

	void onStartFrame(float delta);

	void onEndGuiFrame();

	void onEvent(const WindowResizedEvent& event);

	void onEvent(const MouseMovedEvent& event);

	void onEvent(const KeyEvent& event);

	void onEvent(const TextEvent& event);

	auto getView()
	{
		return mRenderer.getView();
	}
};
} // namespace spatial
