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
	Engine m_engine;
	SwapChain m_swapChain;
	Renderer m_renderer;

	Camera m_mainCamera;
	View m_mainView;

	std::deque<filament::View*> m_views;

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
	void pushFrontView(filament::View* view);

	/**
	 * \brief Deregisters a view to the renderer
	 */
	void popFrontView();

	/**
	 * \brief Registers a view to the renderer
	 */
	void pushBackView(filament::View* view);

	/**
	 * \brief Pops a view to the renderer
	 */
	void popBackView();

	// region Getters
	auto getEngine()
	{
		return m_engine.get();
	}

	auto getMainView()
	{
		return m_mainView.get();
	}

	auto getMainCamera()
	{
		return m_mainCamera.get();
	}

	// endregion
};
} // namespace spatial
