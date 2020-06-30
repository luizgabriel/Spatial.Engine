#include <spatial/ui/UserInterfaceSystem.h>

#include <utility>

namespace fs = std::filesystem;

namespace spatial
{

UserInterfaceSystem::UserInterfaceSystem(RenderingSystem& rendering, const Window& window, fs::path fontPath)
	: mRenderer{rendering.getEngine()}, mFontPath{std::move(fontPath)}
{
	setupViewport(window.getWindowSize(), window.getFrameBufferSize());
	rendering.pushFrontView(getView());
}

void UserInterfaceSystem::onStart()
{
	mRenderer.setup(mFontPath);
	mInput.setup();
}

void UserInterfaceSystem::onEvent(const WindowResizedEvent& event)
{
	setupViewport(event.windowSize, event.frameBufferSize);
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

void UserInterfaceSystem::setupViewport(const std::pair<int, int>& windowSize, const std::pair<int, int>& frameBufferSize)
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