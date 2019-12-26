#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/render/Engine.h>
#include <map>

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

public:
    RenderingSubsystem(desktop::Window&& window);

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

};

} // namespace spatial