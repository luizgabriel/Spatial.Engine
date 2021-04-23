#include <spatial/render/RenderingSystem.h>
#include <spatial/render/Resources.h>

using namespace spatial;
namespace fl = filament;
namespace bk = filament::backend;

namespace spatial
{

RenderingSystem::RenderingSystem(const RenderingSystem::Backend backend, void* nativeWindowHandle)
	: mEngine{createEngine(backend)},
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
		mOnRenderSignal(mRenderer.ref());
		mRenderer->endFrame();
	}
}

} // namespace spatial
