#include <spatial/ui/UserInterfaceSystem.h>

#include <utility>

namespace fs = std::filesystem;

namespace spatial
{

UserInterfaceSystem::UserInterfaceSystem(filament::Engine* engine, fs::path fontPath)
	: m_renderer{engine},
	  m_fontPath{std::move(fontPath)}
{
}

void UserInterfaceSystem::attach(EventQueue& queue)
{
	queue.connect<WindowResizedEvent>(this);
	queue.connect<MouseMovedEvent>(this);
	queue.connect<KeyEvent>(this);
	queue.connect<TextEvent>(this);
}

void UserInterfaceSystem::detach(EventQueue& queue)
{
	queue.disconnect<WindowResizedEvent>(this);
	queue.disconnect<MouseMovedEvent>(this);
	queue.disconnect<KeyEvent>(this);
	queue.disconnect<TextEvent>(this);
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
	auto [w, h] = windowSize;
	auto [fw, fh] = frameBufferSize;
	const auto dpiX = w / fw;
	const auto dpiY = h / fh;

	m_renderer.setViewport(w, h, dpiX, dpiY);
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