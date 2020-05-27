#include <spatial/render/RenderingSystem.h>

using namespace spatial;
namespace fl = filament;
namespace bk = filament::backend;

namespace spatial
{
RenderingSystem::RenderingSystem(const Window& window)
	: RenderingSystem(window, bk::Backend::OPENGL)
{
}

RenderingSystem::RenderingSystem(const Window& window, const bk::Backend backend)
	: m_engine{createEngine(backend)},
	  m_swapChain{createSwapChain(m_engine.get(), window.getNativeHandle())},
	  m_renderer{createRenderer(m_engine.get())},
	  m_mainCamera{createCamera(m_engine.get())},
	  m_mainView{createView(m_engine.get())},
	  m_views{5}
{
	pushBackView(m_mainView.get());
	setupViewport(window.getFrameBufferSize());
}

void RenderingSystem::attach(EventQueue& queue)
{
	queue.connect<WindowResizedEvent>(this);
}

void RenderingSystem::detach(EventQueue& queue)
{
	queue.disconnect<WindowResizedEvent>(this);
}

void RenderingSystem::onStart()
{
	m_mainView->setCamera(m_mainCamera.get());
	// m_mainView->setVisibleLayers(0x04, 0x04);

	m_mainCamera->setExposure(16.0f, 1 / 125.0f, 100.0f);
}

void RenderingSystem::onEndFrame()
{
	if (m_renderer->beginFrame(m_swapChain.get()))
	{
		for (const auto view : m_views)
			m_renderer->render(view);

		m_renderer->endFrame();
	}
}

void RenderingSystem::onEvent(const WindowResizedEvent& event)
{
	this->setupViewport(event.frameBufferSize);
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

void RenderingSystem::setupViewport(const std::pair<int, int>& frameBufferSize)
{
	auto [dw, dh] = frameBufferSize;
	m_mainView->setViewport({0, 0, static_cast<uint32_t>(dw), static_cast<uint32_t>(dh)});
	m_mainCamera->setProjection(45.0, double(dw) / dh, 0.1, 1000000.0f, fl::Camera::Fov::VERTICAL);
}
} // namespace spatial
