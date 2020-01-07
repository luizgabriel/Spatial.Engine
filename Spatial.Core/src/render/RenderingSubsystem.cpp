#include <spatial/render/RenderingSubsystem.h>
#include <spatial/render/Engine.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/core/EBus.h>

using namespace spatial::desktop;
using namespace spatial::core;

namespace spatial::render
{

RenderingSubsystem::RenderingSubsystem(Window &&window)
	: m_window{std::move(window)},
	  m_engine{filament::backend::Backend::OPENGL},
	  m_swapChain{createSwapChain(m_engine, m_window.getNativeHandle())},
	  m_renderer{createRenderer(m_engine)},
	  m_mainView{createView(m_engine)},
	  m_uiView{createView(m_engine)},
	  m_mainCamera{createCamera(m_engine)},
	  m_uiCamera{createCamera(m_engine)}
{
	m_mainView->setCamera(m_mainCamera);
	m_uiView->setCamera(m_uiCamera);

	m_mainCamera->setExposure(16.0f, 1 / 125.0f, 100.0f);
	setupViewport();

	m_mainView->setClearTargets(true, true, true);
	m_mainView->setClearColor({.0f, .0f, .0f, 1.0f});

	EBus::connect<WindowResizedEvent>(this);
}

RenderingSubsystem::~RenderingSubsystem()
{
	EBus::disconnect<WindowResizedEvent>(this);
}

void RenderingSubsystem::onRender()
{
	if (m_renderer->beginFrame(m_swapChain))
	{
		m_renderer->render(m_mainView);
		m_renderer->render(m_uiView);

		m_renderer->endFrame();
	}
}

void RenderingSubsystem::onEvent(const WindowResizedEvent &event)
{
	setupViewport();
}

void RenderingSubsystem::setupViewport()
{
	auto [w, h] = m_window.getFrameBufferSize();
	auto [vw, vh] = m_window.getWindowSize();
	auto dpiX = (float) w / vw;
	auto dpiY = (float) h / vh;

	m_mainView->setViewport({0, 0, w, h});
	m_mainCamera->setProjection(
		45.0, double(w) / h, 0.1, 50,
		filament::Camera::Fov::VERTICAL
	);

	m_uiView->setViewport({0, 0, w, h});
	m_uiCamera->setProjection(
		filament::Camera::Projection::ORTHO,
		0.0, w / dpiX, h / dpiY,
		0.0, 0.0, 1.0
	);
}

} // namespace spatial::render