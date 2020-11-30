#pragma once

#include <spatial/common/EventQueue.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>
#include <spatial/render/Engine.h>
#include <spatial/render/Resources.h>

#include <deque>

namespace spatial
{
class RenderingSystem
{
  public:
	using Backend = filament::backend::Backend;

	RenderingSystem(const Backend backend, void* nativeWindowHandle);
	RenderingSystem(const Backend backend, const Window& window);

	RenderingSystem(const RenderingSystem& other) = delete;
	RenderingSystem& operator=(const RenderingSystem& w) = delete;

	void onStart();

	void onFinish();

	void onEndFrame();

	/**
	 * \brief Registers a view to the renderer
	 */
	void pushFrontView(std::weak_ptr<filament::View> view);

	/**
	 * \brief Deregisters a view to the renderer
	 */
	void popFrontView();

	/**
	 * \brief Registers a view to the renderer
	 */
	void pushBackView(std::weak_ptr<filament::View> view);

	/**
	 * \brief Pops a view to the renderer
	 */
	void popBackView();

    void popView(const SharedView& view);

    [[nodiscard]] const auto& getEngine() const
	{
		return *mEngine.get();
	}

	auto& getEngine()
	{
		return *mEngine.get();
	}


  private:
	Engine mEngine;
	SwapChain mSwapChain;
	Renderer mRenderer;
	filament::Renderer::ClearOptions mClearOptions;

	std::deque<std::weak_ptr<filament::View>> mViews;

	void clearExpiredViews() noexcept;
};
} // namespace spatial
