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
	core::SignalsConnector<self_t> m_signalsConnector;
	core::EventConnector<desktop::WindowResizedEvent, self_t> m_windowResizedEventConnector;

	desktop::Window m_window;
	RenderEngine m_engine;
	SwapChain m_swapChain;
	Renderer m_renderer;

	Camera m_mainCamera;
	filament::View *m_mainView;

	std::vector<filament::View *> m_views;

	void setupViewport();

public:
	RenderingSystem(core::Application *app, desktop::Window &&window);
	~RenderingSystem();

	void onStart();

	void onEndFrame(float delta);

	void onEvent(const desktop::WindowResizedEvent &event);

	/**
	 * \brief Pushes a view to the renderer
	 * The rendering system will now be the owner of the view resource. 
	 */
	void pushView(View &&view);

	/**
	 * \brief Pushes a view to the renderer
	 * The rendering system will now be the owner of the view resource. 
	 * When this system is destructed, pushed views are destroyed.
	 */
	void pushView(filament::View *view);

	//region Getters
	filament::Engine *getEngine()
	{
		return m_engine.get();
	}

	desktop::Window &getWindow()
	{
		return m_window;
	}

	filament::View *getMainView()
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