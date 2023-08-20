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
	: mEngine{createEngine([&](){
        if (nativeWindowHandle == nullptr)
        {
            return bk::Backend::NOOP;
        }

#if defined(SPATIAL_GRAPHICS_BACKEN_OPENGL)
        return bk::Backend::OPENGL;
#elif defined(SPATIAL_GRAPHICS_BACKEND_VULKAN)
        return bk::Backend::VULKAN;
#elif defined(SPATIAL_GRAPHICS_BACKEND_METAL)
        return bk::Backend::METAL;
#else
        static_assert(false, "Unknown backend");
#endif
    }())},
	  mRenderer{createRenderer(getEngine())},
	  mSwapChain{[&]() {
		  if (nativeWindowHandle == nullptr)
		  {
			  return createSwapChain(getEngine(), nullptr);
		  }

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
