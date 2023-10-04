#pragma once

#include <spatial/common/Signal.h>
#include <spatial/graphics/Engine.h>
#include <spatial/graphics/Resources.h>

namespace spatial::graphics
{
using Backend = filament::backend::Backend;

#if defined(SPATIAL_GRAPHICS_BACKEND_OPENGL)
static Backend gDefaultBackend = Backend::OPENGL;
#elif defined(SPATIAL_GRAPHICS_BACKEND_VULKAN)
static Backend gDefaultBackend = Backend::VULKAN;
#elif defined(SPATIAL_GRAPHICS_BACKEND_METAL)
static Backend gDefaultBackend = Backend::METAL;
#else
static_assert(false, "Unknown backend");
#endif

class RenderingSystem
{
  public:
	explicit RenderingSystem(Backend backend = gDefaultBackend);

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

	void onEvent(const desktop::WindowResizedEvent& event);

	void attach(const desktop::Window& window);

	private:
		graphics::Engine mEngine;
		graphics::Renderer mRenderer;
		graphics::SwapChain mSwapChain;
		Signal<filament::Renderer&> mOnRenderSignal;
};

} // namespace spatial::graphics
