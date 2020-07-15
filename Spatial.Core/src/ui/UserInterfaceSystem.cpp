#include <spatial/ui/UserInterfaceSystem.h>

namespace spatial
{

UserInterfaceSystem::UserInterfaceSystem(filament::Engine& engine)
	: mRenderer{engine}, mInput{}
{
}

UserInterfaceSystem::UserInterfaceSystem(RenderingSystem& rendering)
	: UserInterfaceSystem(rendering.getEngine())
{
	rendering.pushFrontView(getView());
}

UserInterfaceSystem::UserInterfaceSystem(RenderingSystem& rendering, const Window& window)
	: UserInterfaceSystem(rendering)
{
	setViewport(window.getWindowSize(), window.getFrameBufferSize());
}

void UserInterfaceSystem::setDefaultFont(const std::span<char> fontData)
{
	mRenderer.setFont(fontData);
}

void UserInterfaceSystem::setDefaultMaterial(const std::span<char> materialData)
{
	mRenderer.setMaterial(materialData);
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

void UserInterfaceSystem::onEndGuiFrame()
{
	mRenderer.dispatchCommands();
}


} // namespace spatial