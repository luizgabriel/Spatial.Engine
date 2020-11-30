#include <spatial/render/RenderingSystem.h>

using namespace spatial;
namespace fl = filament;
namespace bk = filament::backend;

namespace spatial
{

RenderingSystem::RenderingSystem(const RenderingSystem::Backend backend, void* nativeWindowHandle)
	: mEngine{createEngine(backend)},
	  mSwapChain{createSwapChain(getEngine(), nativeWindowHandle)},
	  mRenderer{createRenderer(getEngine())},
	  mViews{},
	  mClearOptions{{.0f, .0f, .0f, 1.0f}, true, false}
{
	mRenderer->setClearOptions(mClearOptions);
}

RenderingSystem::RenderingSystem(const RenderingSystem::Backend backend, const Window& window)
	: RenderingSystem(backend, window.getNativeHandle())
{
}

void RenderingSystem::onStart()
{
	mViews.shrink_to_fit();
	clearExpiredViews();
}

void RenderingSystem::onFinish()
{
	clearExpiredViews();
}

void RenderingSystem::onEndFrame()
{
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

void RenderingSystem::pushFrontView(std::weak_ptr<filament::View> view)
{
	mViews.emplace_back(std::move(view));
}

void RenderingSystem::popFrontView()
{
	mViews.pop_back();
}

void RenderingSystem::pushBackView(std::weak_ptr<filament::View> view)
{
	mViews.emplace_front(std::move(view));
}

void RenderingSystem::popBackView()
{
	mViews.pop_front();
}

void RenderingSystem::popView(const SharedView& view)
{
	std::erase_if(mViews, [&view](std::weak_ptr<filament::View>& v){
		const auto vw = v.lock();
		return vw.get() == view.get();
	});
}

} // namespace spatial
