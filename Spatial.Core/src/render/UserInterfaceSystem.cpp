#include <spatial/render/UserInterfaceSystem.h>
#include <spatial/core/ApplicationEvents.h>

#include <filesystem>

using namespace spatial::core;
using namespace spatial::common;
using namespace spatial::desktop;
namespace fl = filament;
namespace fs = std::filesystem;

namespace spatial::render
{

UserInterfaceSystem::UserInterfaceSystem(Application& app, RenderingSystem& rendering)
	: m_ui{rendering.getEngine()},
	  m_window{&rendering.getWindow()}
{
	connect(app, this);
	connect<desktop::WindowResizedEvent>(app, this);
	rendering.pushView(m_ui.getView());
}

void UserInterfaceSystem::onStart()
{
	setupViewport();
	m_ui.setup(fs::path{"fonts"} / "Roboto-Medium.ttf");
}

void UserInterfaceSystem::onEvent(const WindowResizedEvent &event)
{
	setupViewport();
}

void UserInterfaceSystem::setupViewport()
{
	auto [w, h] = m_window->getWindowSize();
	auto [fw, fh] = m_window->getFrameBufferSize();
	auto dpiX = w / fw;
	auto dpiY = h / fh;

	m_ui.setViewport(w, h, dpiX, dpiY);
}

void UserInterfaceSystem::onStartFrame(float delta)
{
	m_ui.beforeRender(delta);
}

void UserInterfaceSystem::onUpdateFrame(float delta)
{
	m_ui.dispatchCommands();
}

} // namespace spatial::render