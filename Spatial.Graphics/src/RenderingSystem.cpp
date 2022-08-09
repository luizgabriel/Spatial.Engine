#include <spatial/graphics/RenderingSystem.h>
#include <spatial/graphics/Resources.h>

namespace fl = filament;
namespace bk = filament::backend;

namespace spatial::graphics
{

RenderingSystem::RenderingSystem(void* nativeWindowHandle)
	: mEngine{createEngine(nativeWindowHandle ? Backend::OPENGL : Backend::NOOP)},
	  mSwapChain{createSwapChain(getEngine(), nativeWindowHandle)},
	  mRenderer{createRenderer(getEngine())},
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
