#pragma once

#include <spatial/common/EventQueue.h>
#include <spatial/common/Math.h>
#include <spatial/common/Signal.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/resources/FileSystem.h>
#include <spatial/ui/system/ImGuiRenderer.h>

#include <string_view>

namespace spatial::ui
{

class UserInterfaceSystem
{
  private:
	ImGuiRenderer mRenderer;
	Signal<> mRenderGuiSignal;

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
};

} // namespace spatial::ui