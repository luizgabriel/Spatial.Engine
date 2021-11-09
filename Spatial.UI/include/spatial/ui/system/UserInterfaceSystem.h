#pragma once

#include <spatial/ui/system/UserInterfaceInput.h>
#include <spatial/ui/system/UserInterfaceRenderer.h>
#include <spatial/common/Math.h>
#include <spatial/common/Signal.h>
#include <spatial/desktop/PlatformEvent.h>

#include <string_view>

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

	void onStart();

	void onUpdateFrame(float delta);

	void onRender(filament::Renderer& renderer) const;

	void onEvent(const WindowResizedEvent& event);

	void onEvent(const MouseMovedEvent& event);

	void onEvent(const KeyEvent& event);

	void onEvent(const TextEvent& event);

	auto& getRenderer()
	{
		return mRenderer;
	}

	auto& getDrawGuiSignal()
	{
		return mRenderGuiSignal;
	}

	template <typename WindowImpl>
	UserInterfaceSystem(filament::Engine& engine, const WindowImpl& window) : UserInterfaceSystem(engine)
	{
		getRenderer().setViewport(window.getSize(), window.getFrameBufferSize());
	}
};

} // namespace spatial
