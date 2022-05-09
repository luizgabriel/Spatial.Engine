#include <spatial/ui/system/UserInterfaceSystem.h>
#include <spatial/ui/components/DockSpace.h>

namespace spatial
{

UserInterfaceSystem::UserInterfaceSystem(filament::Engine& engine) : mRenderer{engine}, mInput{}
{
}

void UserInterfaceSystem::onStart()
{
	mRenderer.setupEngineTheme();
	mInput.setup();
}

void UserInterfaceSystem::onEvent(const WindowResizedEvent& event)
{
	getRenderer().setViewport(event.windowSize, event.frameBufferSize);
}

void UserInterfaceSystem::onEvent(const MouseMovedEvent& event)
{
	mInput.setMousePosition({event.x, event.y});
}

void UserInterfaceSystem::onEvent(const KeyEvent& event)
{
	mInput.setKey(event.key, event.action);
}

void UserInterfaceSystem::onEvent(const TextEvent& event)
{
	mInput.setText(event.text);
}

void UserInterfaceSystem::onEvent(const MouseScrolledEvent& event)
{
	mInput.setScrollOffset(event.xOffset, event.yOffset);
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
	const auto materialData = fileSystem.readBinary(materialResourcePath);
	assert(!materialData.empty());
	mRenderer.setMaterial(materialData.data(), materialData.size());
}

void UserInterfaceSystem::addFont(FileSystem& fileSystem, std::string_view fontPath)
{
	const auto fontData = fileSystem.readBinary(fontPath);
	assert(!fontData.empty());
	mRenderer.addFont(fontData.data(), fontData.size());
}

} // namespace spatial