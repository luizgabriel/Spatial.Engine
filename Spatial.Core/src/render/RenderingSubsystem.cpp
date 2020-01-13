#include <spatial/render/RenderingSubsystem.h>
#include <spatial/render/Engine.h>
#include <spatial/desktop/PlatformEvent.h>

using namespace spatial::desktop;
using namespace spatial::core;

namespace spatial::render
{

RenderingSubsystem::RenderingSubsystem(Application *app, Window &&window)
	: m_signalsConnector{app, this},
	  m_windowResizedEventConnector{app, this},

	  m_window{std::move(window)},
	  m_engine{filament::backend::Backend::OPENGL},
	  m_swapChain{createSwapChain(m_engine, m_window.getNativeHandle())},
	  m_renderer{createRenderer(m_engine)},
	  m_mainView{createView(m_engine)},
	  m_mainCamera{createCamera(m_engine)},
	  m_ui{m_engine}
{
	m_mainView->setCamera(m_mainCamera);
}

void RenderingSubsystem::onStart()
{
	m_ui.onStart();

	m_mainCamera->setExposure(16.0f, 1 / 125.0f, 100.0f);
	m_mainView->setClearTargets(true, true, true);
	m_mainView->setClearColor({.0f, .0f, .0f, 1.0f});

	setupViewport();
}

void RenderingSubsystem::onStartFrame(float delta)
{
	m_ui.onStartFrame(delta);
}

void RenderingSubsystem::onEndFrame(float delta)
{
	m_ui.render();

	if (m_renderer->beginFrame(m_swapChain))
	{
		m_renderer->render(m_mainView);
		m_renderer->render(m_ui.getView());

		m_renderer->endFrame();
	}
}

void RenderingSubsystem::onEvent(const WindowResizedEvent &event)
{
	setupViewport();
}

void RenderingSubsystem::setupViewport()
{
	auto [w, h] = m_window.getWindowSize();
	auto [dw, dh] = m_window.getFrameBufferSize();

	m_mainView->setViewport({0, 0, dw, dh});
	m_mainCamera->setProjection(
		45.0, double(dw) / dh, 0.1, 50,
		filament::Camera::Fov::VERTICAL);

	auto dpiX = w > 0 ? float(dw) / w : .0f;
	auto dpiY = h > 0 ? float(dh) / h : .0f;
	m_ui.setViewport(w, h, dpiX, dpiY);
}

} // namespace spatial::render