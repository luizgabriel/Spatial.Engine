#pragma once

#include <spatial/common/Signal.h>
#include <spatial/render/Engine.h>
#include <spatial/render/Resources.h>

namespace spatial
{
class RenderingSystem
{
  public:
	using Backend = filament::backend::Backend;

	RenderingSystem(const Backend backend, void* nativeWindowHandle);

	template <typename WindowImpl>
	RenderingSystem(const Backend backend, WindowImpl& window) : RenderingSystem(backend, window.getNativeHandle())
	{
	}

	RenderingSystem(const RenderingSystem& other) = delete;
	RenderingSystem& operator=(const RenderingSystem& w) = delete;

	void onEndFrame();

    const auto& getEngine() const
	{
		return *mEngine.get();
	}

	auto& getEngine()
	{
		return *mEngine.get();
	}

	auto& getRenderer()
	{
		return mRenderer.ref();
	}

	auto& getOnRenderSignal()
	{
		return mOnRenderSignal;
	}

  private:
	Engine mEngine;
	SwapChain mSwapChain;
	Renderer mRenderer;
	Signal<filament::Renderer&> mOnRenderSignal;
};

} // namespace spatial
