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
	  m_mainCamera{createCamera(m_engine)},
	  m_ui{m_engine}
{
	m_mainView->setCamera(m_mainCamera);

#ifdef SPATIAL_PLATFORM_WINDOWS
	m_ui.setNativeWindow(m_window.getNativeHandle());
#endif

	EBus::connect<WindowResizedEvent>(this);
}

RenderingSubsystem::~RenderingSubsystem()
{
	EBus::disconnect<WindowResizedEvent>(this);
}

void RenderingSubsystem::onStart()
{
	m_ui.onStart();

	m_mainCamera->setExposure(16.0f, 1 / 125.0f, 100.0f);
	m_mainView->setClearTargets(true, true, true);
	m_mainView->setClearColor({.0f, .0f, .0f, 1.0f});

	setupViewport();
}

void RenderingSubsystem::beforeRender(float delta)
{
	m_ui.beforeRender(delta);
}

void RenderingSubsystem::render()
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
	auto [w, h] = m_window.getFrameBufferSize();
	auto [vw, vh] = m_window.getWindowSize();
	m_mainView->setViewport({0, 0, w, h});
	m_mainCamera->setProjection(
		45.0, double(w) / h, 0.1, 50,
		filament::Camera::Fov::VERTICAL);

	auto dpiX = (float)w / vw;
	auto dpiY = (float)h / vh;
	m_ui.setViewport(w, h, dpiX, dpiY);
}

} // namespace spatial::render