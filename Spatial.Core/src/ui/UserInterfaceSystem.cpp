#include <spatial/ui/UserInterfaceSystem.h>

#include <utility>

namespace fs = std::filesystem;

namespace spatial
{

UserInterfaceSystem::UserInterfaceSystem(RenderingSystem& rendering, const Window& window, fs::path fontPath)
	: m_renderer{rendering.getEngine()}, m_fontPath{std::move(fontPath)}
{
	setupViewport(window.getWindowSize(), window.getFrameBufferSize());
	rendering.pushFrontView(getView());
}

void UserInterfaceSystem::onStart()
{
	m_renderer.setup(m_fontPath);
	m_input.setup();
}

void UserInterfaceSystem::onEvent(const WindowResizedEvent& event)
{
	setupViewport(event.windowSize, event.frameBufferSize);
}

void UserInterfaceSystem::onEvent(const MouseMovedEvent& event)
{
	m_input.setMousePosition({event.x, event.y});
}

void UserInterfaceSystem::onEvent(const KeyEvent& event)
{
	m_input.setKey(event.key, event.action);
}

void UserInterfaceSystem::onEvent(const TextEvent& event)
{
	m_input.setText(event.text);
}

void UserInterfaceSystem::setupViewport(const std::pair<int, int>& windowSize, const std::pair<int, int>& frameBufferSize)
{
	m_renderer.setViewport(windowSize, frameBufferSize);
}

void UserInterfaceSystem::onStartFrame(float delta)
{
	m_renderer.beforeRender(delta);
}

void UserInterfaceSystem::onEndGuiFrame()
{
	m_renderer.dispatchCommands();
}

} // namespace spatial