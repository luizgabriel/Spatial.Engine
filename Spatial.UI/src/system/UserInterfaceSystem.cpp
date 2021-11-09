#include <spatial/ui/system/UserInterfaceSystem.h>

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

} // namespace spatial