#include <spatial/render/UserInterfaceSystem.h>
#include <spatial/render/ImGuiHelpers.h>

#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/RenderableManager.h>

#include <filesystem>

using namespace spatial::core;
using namespace spatial::common;
using namespace spatial::desktop;
namespace fs = std::filesystem;
namespace fl = filament;

namespace spatial::render
{

UserInterfaceSystem::UserInterfaceSystem(Application *app, RenderingSystem* rendering)
	: m_signalsConnector{app, this},
	  m_windowResizedEventConnector{app, this},
	  m_window{&rendering->getWindow()},
	  m_ui{rendering->getEngine()}
{
	rendering->pushView(m_ui.getView());
}

void UserInterfaceSystem::onStart()
{
	setupViewport();
	m_ui.setup();
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