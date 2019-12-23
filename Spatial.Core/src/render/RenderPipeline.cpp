#include <spatial/render/RenderPipeline.h>

namespace spatial::render
{

RenderPipeline::RenderPipeline(desktop::Window *window, filament::Engine *engine)
	: m_window{window},
	  m_engine{engine},

	  m_swapChain{m_engine, m_engine->createSwapChain(m_window->getNativeHandle())},
	  m_renderer{m_engine, m_engine->createRenderer()},
	  m_scene{m_engine, m_engine->createScene()},
	  m_view{m_engine, m_engine->createView()},
	  m_camera{m_engine, m_engine->createCamera()}
{
	auto [w, h] = m_window->getFrameBufferSize();
	auto [virtualW, virtualH] = m_window->getWindowSize();

	m_camera->setProjection(45.0, double(w) / h, 0.1, 50, filament::Camera::Fov::VERTICAL);

	m_view->setClearColor({.0f, 0.5f, 0.5f, 1.0f});

	m_view->setCamera(m_camera.get());
	m_view->setScene(m_scene.get());

	m_view->setViewport({0, 0, w, h});
	m_view->setClearTargets(false, false, false);
	m_view->setRenderTarget(filament::View::TargetBufferFlags::DEPTH_AND_STENCIL);
	m_view->setPostProcessingEnabled(false);
	m_view->setShadowsEnabled(false);
}

void RenderPipeline::onRender()
{
	m_window->onStartRender();

	if (m_renderer->beginFrame(m_swapChain.get()))
	{
		m_renderer->render(m_view.get_const());
		m_renderer->endFrame();
	}

	m_window->onEndRender();
}

void RenderPipeline::onWindowResized()
{
	auto [w, h] = m_window->getFrameBufferSize();
	m_view->setViewport({0, 0, w, h});
}

} // namespace spatial::render