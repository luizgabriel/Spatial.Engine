#include <spatial/render/RenderingSubsystem.h>
#include <spatial/render/Engine.h>

namespace spatial::render
{

RenderingSubsystem::RenderingSubsystem(desktop::Window&& window)
	: m_window{std::move(window)},
	  m_engine{createEngine()},
	  m_pipeline{&m_window, m_engine}
{
}

void RenderingSubsystem::onRender()
{
	m_pipeline.onRender();
}

void RenderingSubsystem::onWindowResized(const desktop::WindowResizedEvent &event)
{
	m_pipeline.onWindowResized();
}

} // namespace spatial::render