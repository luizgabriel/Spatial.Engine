#pragma once

#include <spatial/common/EventQueue.h>

#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>

#include <spatial/render/Engine.h>
#include <spatial/render/CommonResources.h>

#include <filament/Renderer.h>

#include <deque>

namespace spatial::render
{

class RenderingSystem
{
private:
	desktop::Window m_window;
	RenderEngine m_engine;
	SwapChain m_swapChain;
	Renderer m_renderer;

	Camera m_mainCamera;
	View m_mainView;

	std::deque<filament::View *> m_views;

	void setupViewport();

public:
	RenderingSystem(desktop::Window &&window);

	RenderingSystem(desktop::Window &&window, filament::backend::Backend backend);

	RenderingSystem(const RenderingSystem &other) = delete;

	RenderingSystem(RenderingSystem &&other) = default;

	void attach(common::EventQueue &queue);
	void detach(common::EventQueue &queue);

	void onStart();

	void onEndFrame(float delta);

	void onEvent(const desktop::WindowResizedEvent &event);

	/**
	 * \brief Registers a view to the renderer
	 */
	void pushFrontView(filament::View *view);

	/**
	 * \brief Deregisters a view to the renderer
	 */
	void popFrontView();

	/**
	 * \brief Registers a view to the renderer
	 */
	void pushBackView(filament::View *view);

	/**
	 * \brief Deregisters a view to the renderer
	 */
	void popBackView();

	//region Getters
	auto getEngine()
	{
		return m_engine.get();
	}

	const auto &getWindow()
	{
		return m_window;
	}

	const auto getMainView()
	{
		return m_mainView.get();
	}

	auto getMainCamera()
	{
		return m_mainCamera.get();
	}
	//endregion
};

} // namespace spatial::render