#pragma once

#include <spatial/common/Signal.h>
#include <spatial/graphics/Engine.h>
#include <spatial/graphics/Resources.h>

namespace spatial::graphics
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
	graphics::Engine mEngine;
	graphics::Renderer mRenderer;
	graphics::SwapChain mSwapChain;
	Signal<filament::Renderer&> mOnRenderSignal;
};

} // namespace spatial::graphics
