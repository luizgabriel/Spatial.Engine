#include <spatial/ui/components/DockSpace.h>
#include <spatial/ui/system/ImGuiHelpers.h>
#include <spatial/ui/system/UserInterfaceSystem.h>

namespace spatial::ui
{

UserInterfaceSystem::UserInterfaceSystem(filament::Engine& engine) : mRenderer{engine}
{
}

void UserInterfaceSystem::onStart()
{
	imguiSetupTheme();
	imguiSetupInput();
}

void UserInterfaceSystem::onEvent(const WindowResizedEvent& event)
{
	getRenderer().setViewport(event.windowSize, event.frameBufferSize);
}

void UserInterfaceSystem::onEvent(const MouseMovedEvent& event)
{
	imguiSetMousePosition({event.x, event.y});
}

void UserInterfaceSystem::onEvent(const KeyEvent& event)
{
	imguiSetKey(event.key, event.action);
}

void UserInterfaceSystem::onEvent(const TextEvent& event)
{
	imguiSetText(event.text);
}

void UserInterfaceSystem::onEvent(const MouseScrolledEvent& event)
{
	imguiSetScrollOffset(event.xOffset, event.yOffset);
}

void UserInterfaceSystem::onUpdateFrame(float delta)
{
	mRenderer.initNewFrame(delta);

	{
		auto dockSpace = ui::DockSpace{"Spatial"};
		mRenderGuiSignal();
	}

	mRenderer.drawFrame();
}

void UserInterfaceSystem::onRender(filament::Renderer& renderer) const
{
	renderer.render(&mRenderer.getView());
}

void UserInterfaceSystem::setMaterial(FileSystem& fileSystem, std::string_view materialResourcePath)
{
	auto materialData = fileSystem.readBinary(materialResourcePath);
	assert(!materialData.empty());
	mRenderer.setMaterial(materialData.data(), materialData.size());
}

void UserInterfaceSystem::addFont(FileSystem& fileSystem, std::string_view fontPath)
{
	auto fontData = fileSystem.readBinary(fontPath);
	assert(!fontData.empty());
	mRenderer.addFont(fontData.data(), fontData.size());
}

} // namespace spatial