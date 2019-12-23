#include <spatial/render/RenderingSubsystem.h>

namespace spatial::render
{

RenderingSubsystem::RenderingSubsystem(desktop::Window* window)
	: m_engine{filament::Engine::create(filament::backend::Backend::OPENGL)},
	  m_pipeline{window, m_engine}
{
}

RenderingSubsystem::~RenderingSubsystem()
{
	filament::Engine::destroy(&m_engine);
}

void RenderingSubsystem::onRender()
{
	m_pipeline.onRender();
}

void RenderingSubsystem::onWindowResized(const desktop::WindowResizedEvent &event)
{
	m_pipeline.onWindowResize();
}

} // namespace spatial::render