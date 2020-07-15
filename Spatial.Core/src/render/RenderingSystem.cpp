#include <spatial/render/RenderingSystem.h>

using namespace spatial;
namespace fl = filament;
namespace bk = filament::backend;

namespace spatial
{

RenderingSystem::RenderingSystem(const bk::Backend backend, void* nativeWindowHandle)
	: mEngine{createEngine(backend)},
	  mSwapChain{createSwapChain(getEngine(), nativeWindowHandle)},
	  mRenderer{createRenderer(getEngine())},
	  mMainView{toShared(createView(getEngine()))},
	  mViews{5}
{
	pushBackView(mMainView);
}

RenderingSystem::RenderingSystem(const filament::backend::Backend backend, const Window& window)
	: RenderingSystem(backend, window.getNativeHandle())
{
	setupViewport(window.getFrameBufferSize());
}

void RenderingSystem::onEndFrame()
{
	clearExpiredViews();

	if (mRenderer->beginFrame(mSwapChain.get()))
	{
		for (const auto& view : mViews)
		{
			const auto ownedView = view.lock();
			mRenderer->render(ownedView.get());
		}

		mRenderer->endFrame();
	}
}

void RenderingSystem::clearExpiredViews() noexcept
{
	std::erase_if(mViews, [](auto& view) { return view.expired(); });
}

void RenderingSystem::onEvent(const WindowResizedEvent& event)
{
	this->setupViewport(event.frameBufferSize);
}

void RenderingSystem::pushFrontView(std::weak_ptr<filament::View>&& view)
{
	mViews.emplace_back(std::move(view));
}

void RenderingSystem::popFrontView()
{
	mViews.pop_back();
}

void RenderingSystem::pushBackView(std::weak_ptr<filament::View>&& view)
{
	mViews.emplace_front(std::move(view));
}

void RenderingSystem::popBackView()
{
	mViews.pop_front();
}

void RenderingSystem::setupViewport(const std::pair<int, int>& frameBufferSize)
{
	auto [dw, dh] = frameBufferSize;
	mMainView->setViewport({0, 0, static_cast<uint32_t>(dw), static_cast<uint32_t>(dh)});
}

} // namespace spatial
