#include <spatial/ui/UserInterfaceSystem.h>

using namespace spatial::common;
using namespace spatial::desktop;
using namespace spatial::render;
namespace fl = filament;
namespace fs = std::filesystem;

namespace spatial::ui
{

UserInterfaceSystem::UserInterfaceSystem(RenderingSystem& rendering, const fs::path& fontPath)
	: m_renderer{rendering.getEngine()},
	  m_input{},
	  m_window{&rendering.getWindow()},
	  m_fontPath{fontPath}
{
	rendering.pushFrontView(m_renderer.getView());
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
	setupViewport();

	m_renderer.setup(m_fontPath);
	m_input.setup();
}

void UserInterfaceSystem::onEvent(const WindowResizedEvent &event)
{
	setupViewport();
}

void UserInterfaceSystem::onEvent(const MouseMovedEvent &event)
{
	m_input.setMousePosition({event.x, event.y});
}

void UserInterfaceSystem::onEvent(const KeyEvent &event)
{
	m_input.setKey(event.key, event.action);
}

void UserInterfaceSystem::onEvent(const TextEvent &event)
{
	m_input.setText(event.text);
}

void UserInterfaceSystem::setupViewport()
{
	auto [w, h] = m_window->getWindowSize();
	auto [fw, fh] = m_window->getFrameBufferSize();
	auto dpiX = w / fw;
	auto dpiY = h / fh;

	m_renderer.setViewport(w, h, dpiX, dpiY);
}

void UserInterfaceSystem::onStartFrame(float delta)
{
	m_renderer.beforeRender(delta);
}

void UserInterfaceSystem::onUpdateGuiFrame(float delta)
{
	m_renderer.dispatchCommands();
}

} // namespace spatial::render