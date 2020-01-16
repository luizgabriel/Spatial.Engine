#include <spatial/render/RenderingSystem.h>
#include <spatial/render/Engine.h>
#include <spatial/desktop/PlatformEvent.h>

using namespace spatial::desktop;
using namespace spatial::core;
namespace fl = filament;

namespace spatial::render
{

RenderingSystem::RenderingSystem(Application& app, Window &&window, fl::backend::Backend backend)
	: m_signalsConnector{app, this},
	  m_windowResizedConnector{app, this},

	  m_window{std::move(window)},
	  m_engine{backend},
	  m_swapChain{createSwapChain(m_engine.get(), m_window.getNativeHandle())},
	  m_renderer{createRenderer(m_engine.get())},
	  m_mainCamera{createCamera(m_engine.get())},
	  m_mainView{createSharedResource(m_engine.get(), m_engine.get()->createView())},
	  m_views{5}
{
	registerView(m_mainView);
}

void RenderingSystem::onStart()
{
	m_mainView->setClearTargets(true, true, true);
	m_mainView->setClearColor({.0f, .0f, .0f, 1.0f});
	m_mainView->setCamera(m_mainCamera.get());

	m_mainCamera->setExposure(16.0f, 1 / 125.0f, 100.0f);

	setupViewport();
}

void RenderingSystem::onEndFrame(float delta)
{
	if (m_renderer->beginFrame(m_swapChain.get()))
	{
		for (const auto &view : m_views)
			m_renderer->render(view.get());

		m_renderer->endFrame();
	}
}

void RenderingSystem::onEvent(const WindowResizedEvent &event)
{
	setupViewport();
}

void RenderingSystem::registerView(const SharedView view)
{
	m_views.push_back(view);
}

void RenderingSystem::setupViewport()
{
	auto [dw, dh] = m_window.getFrameBufferSize();

	m_mainView->setViewport({0, 0, dw, dh});
	m_mainCamera->setProjection(
		45.0, double(dw) / dh, 0.1, 50,
		fl::Camera::Fov::VERTICAL);
}

} // namespace spatial::render