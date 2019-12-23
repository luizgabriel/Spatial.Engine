#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/render/RenderPipeline.h>
#include <filament/Engine.h>

namespace spatial::render
{

class RenderingSubsystem
{
private:
	filament::Engine* m_engine;
	RenderPipeline m_pipeline;

public:
    RenderingSubsystem(desktop::Window* window);
	~RenderingSubsystem();

	filament::Engine* getEngine() const
	{
		return m_engine;
	}

	void onRender();
	void onWindowResized(const desktop::WindowResized& event);
};

} // namespace spatial