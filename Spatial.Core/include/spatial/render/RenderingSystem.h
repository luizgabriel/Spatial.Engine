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

	Camera mMainCamera;
	SharedView mMainView;

	std::deque<std::weak_ptr<filament::View>> mViews;

	void setupViewport(const std::pair<int, int>& frameBufferSize);

public:
	explicit RenderingSystem(const Window& window);
	RenderingSystem(const Window& window, filament::backend::Backend backend);

	RenderingSystem(const RenderingSystem& other) = delete;
	RenderingSystem& operator=(const RenderingSystem& w) = delete;

	void onStart();

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

	[[nodiscard]] const filament::Camera& getMainCamera() const
	{
		return *mMainCamera.get();
	}

	auto& getEngine()
	{
		return *mEngine.get();
	}

	auto& getMainView()
	{
		return *mMainView.get();
	}

	auto& getMainCamera()
	{
		return *mMainCamera.get();
	}
	// endregion

private:
	void clearExpiredViews() noexcept;
};
} // namespace spatial
