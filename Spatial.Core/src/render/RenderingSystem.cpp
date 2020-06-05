#include <spatial/render/RenderingSystem.h>
#include <execution>

using namespace spatial;
namespace fl = filament;
namespace bk = filament::backend;

namespace spatial
{
RenderingSystem::RenderingSystem(const Window& window) : RenderingSystem(window, bk::Backend::OPENGL)
{
}

RenderingSystem::RenderingSystem(const Window& window, const bk::Backend backend)
	: m_engine{createEngine(backend)},
	  m_swapChain{createSwapChain(getEngine(), window.getNativeHandle())},
	  m_renderer{createRenderer(getEngine())},
	  m_mainCamera{createCamera(getEngine())},
	  m_mainView{toShared(createView(getEngine()))},
	  m_views{5}
{
	pushBackView(m_mainView);
	setupViewport(window.getFrameBufferSize());
}

void RenderingSystem::onStart()
{
	m_mainView->setCamera(m_mainCamera.get());
	m_mainCamera->setExposure(16.0f, 1 / 125.0f, 100.0f);
}

void RenderingSystem::onEndFrame()
{
	clearExpiredViews();

	if (m_renderer->beginFrame(m_swapChain.get()))
	{
		for (const auto& view : m_views)
		{
			const auto ownedView = view.lock();
			m_renderer->render(ownedView.get());
		}

		m_renderer->endFrame();
	}
}

void RenderingSystem::clearExpiredViews() noexcept
{
	m_views.erase(
		std::remove_if(std::execution::par_unseq, m_views.begin(), m_views.end(), [](auto& view) { return view.expired(); }),
		m_views.end());
}

void RenderingSystem::onEvent(const WindowResizedEvent& event)
{
	this->setupViewport(event.frameBufferSize);
}

void RenderingSystem::pushFrontView(std::weak_ptr<filament::View>&& view)
{
	m_views.emplace_back(std::move(view));
}

void RenderingSystem::popFrontView()
{
	m_views.pop_back();
}

void RenderingSystem::pushBackView(std::weak_ptr<filament::View>&& view)
{
	m_views.emplace_front(std::move(view));
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
