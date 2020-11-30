#pragma once

#include <spatial/render/Resources.h>
#include <spatial/common/Signal.h>
#include <utility>
#include <string_view>

namespace spatial::editor
{

class ImGuiSceneWindow
{
  public:
	using Size = std::pair<uint32_t, uint32_t>;

	ImGuiSceneWindow(filament::Engine& engine, Size size);

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

	auto& getWindowResizedSignal()
	{
		return mWindowResizedSignal;
	}

  private:
	Texture mRenderColorTexture;
	Texture mRenderDepthTexture;
	RenderTarget mRenderTarget;
	Size mLastWindowSize;
	Signal<Size> mWindowResizedSignal;
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

