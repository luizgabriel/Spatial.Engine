#include <spatial/render/RenderingSystem.h>
#include <spatial/render/Engine.h>
#include <spatial/desktop/PlatformEvent.h>

using namespace spatial::desktop;
using namespace spatial::core;

namespace spatial::render
{

RenderingSystem::RenderingSystem(Application *app, Window &&window)
	: m_signalsConnector{app, this},
	  m_windowResizedEventConnector{app, this},

	  m_window{std::move(window)},
	  m_engine{filament::backend::Backend::OPENGL},
	  m_swapChain{createSwapChain(m_engine.get(), m_window.getNativeHandle())},
	  m_renderer{createRenderer(m_engine.get())},
	  m_mainCamera{createCamera(m_engine.get())},
	  m_mainView{m_engine.createView()},
	  m_views{5}
{
	pushView(m_mainView);
}

RenderingSystem::~RenderingSystem()
{
	auto& engine = m_engine.ref();

	for (auto viewPtr : m_views)
		engine.destroy(viewPtr);
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
		for (auto &view : m_views)
			m_renderer->render(view);

		m_renderer->endFrame();
	}
}

void RenderingSystem::onEvent(const WindowResizedEvent &event)
{
	setupViewport();
}

void RenderingSystem::pushView(View&& view)
{
	m_views.push_back(view.extract());
}

void RenderingSystem::pushView(filament::View *view)
{
	m_views.push_back(view);
}

void RenderingSystem::setupViewport()
{
	auto [dw, dh] = m_window.getFrameBufferSize();

	m_mainView->setViewport({0, 0, dw, dh});
	m_mainCamera->setProjection(
		45.0, double(dw) / dh, 0.1, 50,
		filament::Camera::Fov::VERTICAL);
}

} // namespace spatial::render