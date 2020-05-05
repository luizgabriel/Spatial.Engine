#pragma once

#include <spatial/common/EventQueue.h>

#include <spatial/desktop/PlatformEvent.h>

#include <spatial/render/Engine.h>
#include <spatial/render/CommonResources.h>

#include <filament/Viewport.h>

#include <deque>

namespace spatial
{
class RenderingSystem
{
private:
	RenderEngine m_engine;
	SwapChain m_swapChain;
	Renderer m_renderer;

	Camera m_mainCamera;
	View m_mainView;

	std::deque<filament::View*> m_views;

public:
	explicit RenderingSystem(void* nativeWindowHandle);
	RenderingSystem(void* nativeWindowHandle, filament::backend::Backend backend);

	RenderingSystem(const RenderingSystem& other) = delete;
	RenderingSystem& operator=(const RenderingSystem& w) = delete;

	void attach(EventQueue& queue);
	void detach(EventQueue& queue);

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
	auto getEngine() { return m_engine.get(); }

	auto getMainView() { return m_mainView.get(); }

	auto getMainCamera() { return m_mainCamera.get(); }

	void setupViewport(const std::pair<int, int>& frameBufferSize);
	// endregion
};
} // namespace spatial
