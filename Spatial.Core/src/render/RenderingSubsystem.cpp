#include <spatial/render/RenderingSubsystem.h>
#include <spatial/render/Engine.h>

namespace spatial::render
{

RenderingSubsystem::RenderingSubsystem(desktop::Window&& window)
	: m_window{std::move(window)},
	  m_engine{filament::backend::Backend::OPENGL},
	  m_swapChain{m_engine.createSwapChain(m_window.getNativeHandle())},
	  m_renderer{m_engine.createRenderer()},
	  m_mainView{m_engine.createView()}
{
}

void RenderingSubsystem::onRender()
{
	m_window.onStartRender();
	
	if (m_renderer->beginFrame(m_swapChain.get()))
	{
		m_renderer->render(m_mainView.get());
		
		m_renderer->endFrame();
	} 

	m_window.onEndRender();
}

} // namespace spatial::render