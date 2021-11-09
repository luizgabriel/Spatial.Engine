#include <spatial/render/RenderingSystem.h>
#include <spatial/render/Resources.h>

namespace fl = filament;
namespace bk = filament::backend;

namespace spatial
{

RenderingSystem::RenderingSystem(void* nativeWindowHandle)
	: mEngine{render::createEngine(Backend::OPENGL)},
	  mSwapChain{render::createSwapChain(getEngine(), nativeWindowHandle)},
	  mRenderer{render::createRenderer(getEngine())},
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

} // namespace spatial::render
