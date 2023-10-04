#if defined(SPATIAL_PLATFORM_OSX)
#include <spatial/graphics/native/CocoaHelper.h>
#endif

#include "spatial/desktop/PlatformEvent.h"
#include "spatial/desktop/Window.h"
#include <spatial/graphics/RenderingSystem.h>
#include <spatial/graphics/Resources.h>

namespace fl = filament;
namespace bk = filament::backend;


namespace spatial::graphics
{

RenderingSystem::RenderingSystem(Backend backend)
	: mEngine{createEngine(backend)},
	  mRenderer{createRenderer(getEngine())},
	  mSwapChain{getEngine(), nullptr},
	  mOnRenderSignal{}
{
	mRenderer->setClearOptions({{.0f, .0f, .0f, 1.0f}, true, false});
}

void RenderingSystem::onEndFrame()
{
	if (mRenderer->beginFrame(mSwapChain.get()))
	{
		mOnRenderSignal(*mRenderer);
		mRenderer->endFrame();
	}
}

void RenderingSystem::onEvent(const desktop::WindowResizedEvent& event) {
	#if defined(SPATIAL_PLATFORM_OSX)
		auto* nativeHandle = event.window->getNativeHandle();
		assert(nativeHandle != nullptr);
		::cocoaResizeMetalLayer(::cocoaGetContentViewFromWindow(nativeHandle));
	#endif
}

void RenderingSystem::attach(const desktop::Window& window)
{
	auto nativeWindowHandle = window.getNativeHandle();
	assert(nativeWindowHandle != nullptr);

	mSwapChain = [&]() {
#if defined(SPATIAL_PLATFORM_OSX)
		::cocoaPrepareWindowColorSpace(nativeWindowHandle);

		auto* metalLayer = ::cocoaSetUpMetalLayer(::cocoaGetContentViewFromWindow(nativeWindowHandle));
		return createSwapChain(getEngine(), metalLayer);
#else
		return createSwapChain(getEngine(), nativeWindowHandle);
#endif
	}();
}

} // namespace spatial::graphics
