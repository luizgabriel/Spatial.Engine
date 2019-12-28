#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/render/Engine.h>

namespace spatial::render
{

class RenderingSubsystem
{
private:
	desktop::Window m_window;
	RenderEngine m_engine;
	SwapChain m_swapChain;
	Renderer m_renderer;

	View m_mainView;
	Camera m_mainCamera;

public:
    RenderingSubsystem(desktop::Window&& window);

	~RenderingSubsystem();

	void onRender();

	RenderEngine& getEngine()
	{
		return m_engine;
	}

	const RenderEngine& getEngine() const
	{
		return m_engine;
	}

	desktop::Window& getWindow()
	{
		return m_window;
	}

	const desktop::Window& getWindow() const
	{
		return m_window;
	}

	View& getMainView()
	{
		return m_mainView;
	}

	const View& getMainView() const
	{
		return m_mainView;
	}

	Camera& getMainCamera()
	{
		return m_mainCamera;
	}

	const Camera& getMainCamera() const
	{
		return m_mainCamera;
	}

	void onEvent(const desktop::WindowResizedEvent& event);

};

} // namespace spatial