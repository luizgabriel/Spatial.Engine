#pragma once

#include <spatial/render/Resources.h>
#include <spatial/common/Signal.h>
#include <spatial/common/Math.h>
#include <string_view>

namespace spatial::ui
{

class ImGuiSceneWindow
{
  public:
	ImGuiSceneWindow(filament::Engine& engine, math::int2 size);

	void draw(const std::string_view windowTitle);

	const auto* getColorTexture() const
	{
		return mRenderColorTexture.get();
	}

	const auto* getDepthTexture() const
	{
		return mRenderDepthTexture.get();
	}

	const auto* getRenderTarget() const
	{
		return mRenderTarget.get();
	}

	auto* getRenderTarget()
	{
		return mRenderTarget.get();
	}

	const auto& getWindowSize()
	{
		return mWindowSize;
	}

	double getAspectRatio()
	{
		return mWindowSize.x / static_cast<double>(mWindowSize.y);
	}

	auto& getWindowResizedSignal()
	{
		return mWindowResizedSignal;
	}

  private:
	render::Texture mRenderColorTexture;
	render::Texture mRenderDepthTexture;
	render::RenderTarget mRenderTarget;
	math::int2 mWindowSize;
	Signal<math::int2> mWindowResizedSignal;
};

template <typename Listener>
void connect(ImGuiSceneWindow& uiElement, Listener& listener)
{
	uiElement.getWindowResizedSignal().connect<&Listener::onSceneWindowResized>(listener);
}

template <typename Listener>
ImGuiSceneWindow& operator >>(ImGuiSceneWindow& uiElement, Listener& listener)
{
	connect(uiElement, listener);
	return uiElement;
}

}

