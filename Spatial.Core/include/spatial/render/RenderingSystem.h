#pragma once

#include <spatial/core/Application.h>

#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>

#include <spatial/render/Engine.h>
#include <spatial/render/CommonResources.h>

#include <filament/Renderer.h>

#include <vector>

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

	std::vector<filament::View*> m_views;

	void setupViewport();

public:
	RenderingSystem(core::Application &app, desktop::Window &&window);

	RenderingSystem(core::Application &app, desktop::Window &&window, filament::backend::Backend backend);

	void onStart();

	void onEndFrame(float delta);

	void onEvent(const desktop::WindowResizedEvent &event);

	/**
	 * \brief Register a view to the renderer
	 */
	void pushView(filament::View* view);

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