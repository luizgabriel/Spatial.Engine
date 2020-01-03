#include <spatial/render/RenderingSubsystem.h>
#include <spatial/render/Engine.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/common/EBus.h>

namespace spatial::render
{

RenderingSubsystem::RenderingSubsystem(desktop::Window&& window)
	: m_window{std::move(window)},
	  m_engine{filament::backend::Backend::OPENGL},
	  m_swapChain{createSwapChain(m_engine, m_window.getNativeHandle())},
	  m_renderer{createRenderer(m_engine)},
	  m_mainView{createView(m_engine)},
	  m_mainCamera{createCamera(m_engine)}
{
	m_mainView->setCamera(m_mainCamera.get());

	common::EBus::connect<desktop::WindowResizedEvent>(this);
}

RenderingSubsystem::~RenderingSubsystem()
{
	common::EBus::disconnect<desktop::WindowResizedEvent>(this);
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

void RenderingSubsystem::onEvent(const desktop::WindowResizedEvent& event)
{
	m_mainView->setViewport({0, 0, static_cast<uint32_t>(event.width),  static_cast<uint32_t>(event.height)});
	m_mainCamera->setProjection(45, event.getRatio(), 1.0, 10.0);
}

} // namespace spatial::render