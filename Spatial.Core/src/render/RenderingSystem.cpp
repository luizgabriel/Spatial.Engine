#include <spatial/render/RenderingSystem.h>

#ifndef SPATIAL_PLATFORM_OSX
#include <execution>
#endif

using namespace spatial;
namespace fl = filament;
namespace bk = filament::backend;

namespace spatial
{

RenderingSystem::RenderingSystem(const Window& window) : RenderingSystem(window, bk::Backend::OPENGL)
{
}

RenderingSystem::RenderingSystem(const Window& window, const bk::Backend backend)
	: mEngine{createEngine(backend)},
	  mSwapChain{createSwapChain(getEngine(), window.getNativeHandle())},
	  mRenderer{createRenderer(getEngine())},
	  mMainCamera{createCamera(getEngine())},
	  mMainView{toShared(createView(getEngine()))},
	  mViews{5}
{
	pushBackView(mMainView);
	setupViewport(window.getFrameBufferSize());
}

void RenderingSystem::onStart()
{
	mMainView->setCamera(mMainCamera.get());
	mMainCamera->setExposure(16.0f, 1 / 125.0f, 100.0f);
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
	mViews.erase(
#ifndef SPATIAL_PLATFORM_OSX
		std::remove_if(std::execution::par_unseq, mViews.begin(), mViews.end(),
#else
		std::remove_if(mViews.begin(), mViews.end(),
#endif

					   [](auto& view) { return view.expired(); }),
		mViews.end());
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
	mMainCamera->setProjection(45.0, double(dw) / dh, 0.1, 1000000.0f, fl::Camera::Fov::VERTICAL);
}

} // namespace spatial
