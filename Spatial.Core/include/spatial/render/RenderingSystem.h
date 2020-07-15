#pragma once

#include <spatial/common/EventQueue.h>

#include <spatial/desktop/PlatformEvent.h>

#include <spatial/render/Engine.h>
#include <spatial/render/Resources.h>

#include <filament/Viewport.h>

#include <deque>
#include <spatial/desktop/Window.h>

namespace spatial
{
class RenderingSystem
{
private:
	Engine mEngine;
	SwapChain mSwapChain;
	Renderer mRenderer;
	SharedView mMainView;

	std::deque<std::weak_ptr<filament::View>> mViews;
public:
	RenderingSystem(const filament::backend::Backend backend, void* nativeWindowHandle);
	RenderingSystem(const filament::backend::Backend backend, const Window& window);

	RenderingSystem(const RenderingSystem& other) = delete;
	RenderingSystem& operator=(const RenderingSystem& w) = delete;

	void setupViewport(const std::pair<int, int>& frameBufferSize);

	void onEndFrame();

	void onEvent(const WindowResizedEvent& event);

	/**
	 * \brief Registers a view to the renderer
	 */
	void pushFrontView(std::weak_ptr<filament::View>&& view);

	/**
	 * \brief Deregisters a view to the renderer
	 */
	void popFrontView();

	/**
	 * \brief Registers a view to the renderer
	 */
	void pushBackView(std::weak_ptr<filament::View>&& view);

	/**
	 * \brief Pops a view to the renderer
	 */
	void popBackView();

	// region Getters
	[[nodiscard]] const auto& getEngine() const
	{
		return *mEngine.get();
	}

	[[nodiscard]] const filament::View& getMainView() const
	{
		return *mMainView.get();
	}

	auto& getEngine()
	{
		return *mEngine.get();
	}

	auto& getMainView()
	{
		return *mMainView.get();
	}
	// endregion

private:
	void clearExpiredViews() noexcept;
};
} // namespace spatial
