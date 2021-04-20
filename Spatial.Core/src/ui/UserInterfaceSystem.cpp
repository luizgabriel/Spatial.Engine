#include <spatial/ui/UserInterfaceSystem.h>

namespace spatial
{

UserInterfaceSystem::UserInterfaceSystem(filament::Engine& engine) : mRenderer{engine}, mInput{}
{
}

void UserInterfaceSystem::setFontTexture(const SharedTexture& fontTexture)
{
	mRenderer.setFontTexture(fontTexture);
}

void UserInterfaceSystem::setMaterial(const SharedMaterial& material)
{
	mRenderer.setMaterial(material);
}

void UserInterfaceSystem::onStart()
{
	mRenderer.setupEngineTheme();
	mInput.setup();
}

void UserInterfaceSystem::onEvent(const WindowResizedEvent& event)
{
	setViewport(event.windowSize, event.frameBufferSize);
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

void UserInterfaceSystem::setViewport(const std::pair<int, int>& windowSize, const std::pair<int, int>& frameBufferSize)
{
	mRenderer.setViewport(windowSize, frameBufferSize);
}

void UserInterfaceSystem::onStartFrame(float delta)
{
	mRenderer.beforeRender(delta);
}

void UserInterfaceSystem::onUpdateFrame(float)
{
	mRenderGuiSignal();
	mRenderer.dispatchCommands();
}

void UserInterfaceSystem::onRender(filament::Renderer& renderer) const
{
	renderer.render(&getView());
}

} // namespace spatial