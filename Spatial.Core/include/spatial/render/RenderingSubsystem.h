#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/render/Engine.h>
#include <spatial/render/CommonResources.h>
#include <spatial/render/UserInterfaceRenderer.h>
#include <spatial/core/ApplicationConnector.h>

namespace spatial::render
{

class RenderingSubsystem
{
	using self_t = RenderingSubsystem;

private:
	core::SignalsConnector<self_t> m_signalsConnector;
	core::EventConnector<desktop::WindowResizedEvent, self_t> m_windowResizedEventConnector;

	desktop::Window m_window;
	RenderEngine m_engine;
	SwapChain m_swapChain;
	Renderer m_renderer;

	View m_mainView;
	Camera m_mainCamera;

	UserInterfaceRenderer m_ui;

	void setupViewport();

public:
	RenderingSubsystem(core::Application* app, desktop::Window &&window);

	void onStart();

	void onStartFrame(float delta);
	void onEndFrame(float delta);

	filament::Engine *getEngine()
	{
		return m_engine;
	}

	const filament::Engine *getEngine() const
	{
		return m_engine.get();
	}

	desktop::Window &getWindow()
	{
		return m_window;
	}

	const desktop::Window &getWindow() const
	{
		return m_window;
	}

	filament::View *getMainView()
	{
		return m_mainView.get();
	}

	const filament::View *getMainView() const
	{
		return m_mainView.get();
	}

	filament::Camera *getMainCamera()
	{
		return m_mainCamera.get();
	}

	const filament::Camera *getMainCamera() const
	{
		return m_mainCamera.get();
	}

	void onEvent(const desktop::WindowResizedEvent &event);
};

} // namespace spatial::render