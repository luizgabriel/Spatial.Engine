#include <spatial/render/RenderingSystem.h>

using namespace spatial::desktop;
using namespace spatial::common;
namespace fl = filament;
namespace bk = filament::backend;

namespace spatial::render
{

RenderingSystem::RenderingSystem(Window &&window)
	: RenderingSystem(std::move(window), bk::Backend::OPENGL)
{
}

RenderingSystem::RenderingSystem(Window &&window, bk::Backend backend)
	: m_window{std::move(window)},
	  m_engine{backend},
	  m_swapChain{createSwapChain(m_engine.get(), m_window.getNativeHandle())},
	  m_renderer{createRenderer(m_engine.get())},
	  m_mainCamera{createCamera(m_engine.get())},
	  m_mainView{createView(m_engine.get())},
	  m_views{5}
{
	pushBackView(m_mainView.get());
}

void RenderingSystem::attach(EventQueue& queue)
{
    queue.connect<desktop::WindowResizedEvent>(this);
}

void RenderingSystem::detach(EventQueue& queue)
{
    queue.disconnect<desktop::WindowResizedEvent>(this);
}

void RenderingSystem::onStart()
{
	m_mainView->setClearTargets(true, true, true);
	m_mainView->setClearColor({.0f, .0f, .0f, 1.0f});
	m_mainView->setCamera(m_mainCamera.get());
	//m_mainView->setVisibleLayers(0x04, 0x04);

	m_mainCamera->setExposure(16.0f, 1 / 125.0f, 100.0f);

	setupViewport();
}

void RenderingSystem::onEndFrame(float delta)
{
	if (m_renderer->beginFrame(m_swapChain.get()))
	{
		for (const auto view : m_views)
			m_renderer->render(view);

		m_renderer->endFrame();
	}
}

void RenderingSystem::onEvent(const WindowResizedEvent &event)
{
	setupViewport();
}

void RenderingSystem::setupViewport()
{
	auto [dw, dh] = m_window.getFrameBufferSize();

	m_mainView->setViewport({0, 0, dw, dh});
	m_mainCamera->setProjection(
		45.0, double(dw) / dh, 0.1, 1000000.0f,
		fl::Camera::Fov::VERTICAL);
}

void RenderingSystem::pushFrontView(filament::View* view)
{
	m_views.emplace_back(view);
}

void RenderingSystem::popFrontView()
{
	m_views.pop_back();
}

void RenderingSystem::pushBackView(filament::View* view)
{
	m_views.emplace_front(view);
}

void RenderingSystem::popBackView()
{
	m_views.pop_front();
}

} // namespace spatial::render