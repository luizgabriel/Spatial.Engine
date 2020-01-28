#include <spatial/ui/UserInterfaceSystem.h>
#include <spatial/core/ApplicationEvents.h>

#include <filesystem>

using namespace spatial::core;
using namespace spatial::common;
using namespace spatial::desktop;
using namespace spatial::render;
namespace fl = filament;
namespace fs = std::filesystem;

namespace spatial::ui
{

UserInterfaceSystem::UserInterfaceSystem(Application& app, RenderingSystem& rendering)
	: m_renderer{rendering.getEngine()},
	  m_input{},
	  m_window{&rendering.getWindow()}
{
	connect(app, this);
	connect<WindowResizedEvent>(app, this);
	connect<MouseMovedEvent>(app, this);
	connect<KeyEvent>(app, this);
	connect<TextEvent>(app, this);

	rendering.pushView(m_renderer.getView());
}

void UserInterfaceSystem::onStart()
{
	setupViewport();

	m_renderer.setup(fs::path{"fonts"} / "Roboto-Medium.ttf");
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

void UserInterfaceSystem::onUpdateFrame(float delta)
{
	m_renderer.dispatchCommands();
}

} // namespace spatial::render