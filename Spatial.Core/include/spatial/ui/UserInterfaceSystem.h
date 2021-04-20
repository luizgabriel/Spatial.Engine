#pragma once

#include <spatial/common/Signal.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/ui/UserInterfaceInput.h>
#include <spatial/ui/UserInterfaceRenderer.h>

#include <string_view>
#include <utility>

namespace spatial
{
class UserInterfaceSystem
{
  private:
	UserInterfaceRenderer mRenderer;
	UserInterfaceInput mInput;
	Signal<> mRenderGuiSignal;

  public:
	explicit UserInterfaceSystem(filament::Engine& engine);

	template <typename WindowImpl>
	UserInterfaceSystem(filament::Engine& engine, const WindowImpl& window) : UserInterfaceSystem(engine)
	{
		setViewport(window.getWindowSize(), window.getFrameBufferSize());
	}

	void setFontTexture(const SharedTexture& fontTexture);

	void setMaterial(const SharedMaterial& material);

	void setViewport(const std::pair<int, int>& windowSize, const std::pair<int, int>& frameBufferSize);

	void onStart();

	void onStartFrame(float delta);

	void onUpdateFrame(float delta);

	void onRender(filament::Renderer& renderer) const;

	void onEvent(const WindowResizedEvent& event);

	void onEvent(const MouseMovedEvent& event);

	void onEvent(const KeyEvent& event);

	void onEvent(const TextEvent& event);

	const auto& getView() const
	{
		return mRenderer.getView();
	}

	auto& getDrawGuiSignal()
	{
		return mRenderGuiSignal;
	}
};

} // namespace spatial
