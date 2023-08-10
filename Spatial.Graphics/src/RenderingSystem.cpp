#if defined(SPATIAL_PLATFORM_OSX)
#include <spatial/graphics/native/CocoaHelper.h>
#endif

#include <spatial/graphics/RenderingSystem.h>
#include <spatial/graphics/Resources.h>

namespace fl = filament;
namespace bk = filament::backend;

namespace spatial::graphics
{

RenderingSystem::RenderingSystem(void* nativeWindowHandle)
	: mEngine{createEngine()},
	  mRenderer{createRenderer(getEngine())},
	  mSwapChain{[&]() {
#if defined(SPATIAL_PLATFORM_OSX)
		  ::cocoaPrepareWindowColorSpace(nativeWindowHandle);

		  auto* metalLayer = ::cocoaSetUpMetalLayer(nativeWindowHandle);
		  return createSwapChain(getEngine(), metalLayer);
#else
		  return createSwapChain(getEngine(), nativeWindowHandle);
#endif
	  }()},
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

} // namespace spatial::graphics
