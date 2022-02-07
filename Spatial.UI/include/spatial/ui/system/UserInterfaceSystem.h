#pragma once

#include <spatial/common/Math.h>
#include <spatial/common/Signal.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/ui/system/ImGuiRenderer.h>
#include <spatial/ui/system/UserInterfaceInput.h>
#include <spatial/resources/FilesSystem.h>

#include <string_view>

namespace spatial
{
class UserInterfaceSystem
{
  private:
	FileSystem& mFileSystem;
	ImGuiRenderer mRenderer;
	UserInterfaceInput mInput;
	Signal<> mRenderGuiSignal;

  public:
	explicit UserInterfaceSystem(filament::Engine& engine, FileSystem& fileSystem);

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
	UserInterfaceSystem(filament::Engine& engine, FileSystem& fileSystem, const WindowImpl& window) : UserInterfaceSystem(engine, fileSystem)
	{
		getRenderer().setViewport(window.getSize(), window.getFrameBufferSize());
	}
	void setMaterial(std::string_view materialResourcePath);
	void addFont(std::string_view fontPath);
};

} // namespace spatial
