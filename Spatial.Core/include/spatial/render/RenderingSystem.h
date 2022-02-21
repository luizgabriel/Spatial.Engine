#pragma once

#include <spatial/common/Signal.h>
#include <spatial/render/Engine.h>
#include <spatial/render/Resources.h>

namespace spatial::render
{

class RenderingSystem
{
  public:
	using Backend = filament::backend::Backend;

	explicit RenderingSystem(void* nativeWindowHandle);

	template <typename WindowImpl>
	explicit RenderingSystem(WindowImpl& window) : RenderingSystem(window.getNativeHandle())
	{
	}

	RenderingSystem(const RenderingSystem& other) = delete;
	RenderingSystem& operator=(const RenderingSystem& w) = delete;

	void onEndFrame();

	[[nodiscard]] const auto& getEngine() const
	{
		return *mEngine.get();
	}

	auto& getEngine()
	{
		return *mEngine.get();
	}

	auto& getOnRenderSignal()
	{
		return mOnRenderSignal;
	}

  private:
	render::Engine mEngine;
	render::SwapChain mSwapChain;
	render::Renderer mRenderer;
	Signal<filament::Renderer&> mOnRenderSignal;
};

} // namespace spatial
