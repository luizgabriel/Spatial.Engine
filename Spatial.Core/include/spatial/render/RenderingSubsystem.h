#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/render/RenderPipeline.h>
#include <spatial/render/Engine.h>

namespace spatial::render
{

class RenderingSubsystem
{
private:
	desktop::Window m_window;
	RenderEngine m_engine;
	RenderPipeline m_pipeline;

public:
    RenderingSubsystem(desktop::Window&& window);

	const RenderEngine* getEngine() const
	{
		return &m_engine;
	}

	const RenderPipeline* getRenderPipeline() const
	{
		return &m_pipeline;
	}

	void onRender();
	void onWindowResized(const desktop::WindowResizedEvent& event);
};

} // namespace spatial