#pragma once

#include <spatial/common/Math.h>
#include <spatial/common/Signal.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/resources/FileSystem.h>
#include <spatial/ui/system/ImGuiRenderer.h>

#include <string_view>

namespace spatial::ui
{

struct ClipboardController
{
   std::function<std::optional<std::string>()> get;
   std::function<void(const std::string&)> set;
};

template <typename T>
ClipboardController getClipboardControllerFromWindow(T& window) {
    return {
        [&window](){ return window.getClipboardText(); },
        [&window](const std::string& text){ window.setClipboardText(text); }
    };
}

class UserInterfaceSystem
{
  private:
	ImGuiRenderer mRenderer;
	Signal<> mRenderGuiSignal;
    ClipboardController mClipboardController{[](){ return std::nullopt; }, [](const std::string&){}};

  public:
	explicit UserInterfaceSystem(filament::Engine& engine);

	void onStart();

	void onUpdateFrame(float delta);

	void onRender(filament::Renderer& renderer) const;

	void onEvent(const WindowResizedEvent& event);

	void onEvent(const MouseMovedEvent& event);

	void onEvent(const MouseScrolledEvent& event);

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

	void setMaterial(FileSystem& fileSystem, std::string_view materialResourcePath);

	void addFont(FileSystem& fileSystem, std::string_view fontPath);

    void setClipboardController(ClipboardController controller);
};

} // namespace spatial::ui
