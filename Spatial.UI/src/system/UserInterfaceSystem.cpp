#include <spatial/ui/system/UserInterfaceSystem.h>

namespace spatial
{

UserInterfaceSystem::UserInterfaceSystem(filament::Engine& engine, FileSystem& fileSystem)  : mFileSystem{fileSystem}, mRenderer{engine}, mInput{}
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

void UserInterfaceSystem::onUpdateFrame(float delta)
{
	mRenderer.initNewFrame(delta);
	mRenderGuiSignal();
	mRenderer.drawFrame();
}

void UserInterfaceSystem::onRender(filament::Renderer& renderer) const
{
	renderer.render(&mRenderer.getView());
}

void UserInterfaceSystem::setMaterial(std::string_view materialResourcePath)
{
	auto materialData = mFileSystem.readBinary(materialResourcePath);
	mRenderer.setMaterial(materialData.data(), materialData.size());
}

void UserInterfaceSystem::addFont(std::string_view fontPath)
{
	auto fontData = mFileSystem.readBinary(fontPath);
	mRenderer.addFont(fontData.data(), fontData.size());
}

} // namespace spatial