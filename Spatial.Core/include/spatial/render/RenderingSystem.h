#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/render/Engine.h>
#include <spatial/render/CommonResources.h>
#include <spatial/core/ApplicationConnector.h>

#include <vector>

namespace spatial::render
{

class RenderingSystem
{
	using self_t = RenderingSystem;

private:
	core::AppSignalsConnector<self_t> m_signalsConnector;
	core::AppEventConnector<desktop::WindowResizedEvent, self_t> m_windowResizedConnector;

	desktop::Window m_window;
	RenderEngine m_engine;
	SwapChain m_swapChain;
	Renderer m_renderer;

	Camera m_mainCamera;
	SharedView m_mainView;

	std::vector<SharedView> m_views;

	void setupViewport();

public:
	RenderingSystem(core::Application &app, desktop::Window &&window)
		: RenderingSystem::RenderingSystem(app, std::move(window), filament::backend::Backend::OPENGL)
	{
	}

	RenderingSystem(core::Application &app, desktop::Window &&window, filament::backend::Backend backend);

	void onStart();

	void onEndFrame(float delta);

	void onEvent(const desktop::WindowResizedEvent &event);

	/**
	 * \brief Pushes a view to the renderer
	 */
	void registerView(const SharedView view);

	//region Getters
	filament::Engine *getEngine()
	{
		return m_engine.get();
	}

	const desktop::Window &getWindow()
	{
		return m_window;
	}

	const SharedView getMainView()
	{
		return m_mainView;
	}

	filament::Camera *getMainCamera()
	{
		return m_mainCamera.get();
	}
	//endregion
};

} // namespace spatial::render