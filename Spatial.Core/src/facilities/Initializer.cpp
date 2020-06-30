#include <spatial/facilities/Initializer.h>
#include <spatial/core/System.h>
#include <spatial/input/InputSystem.h>
#include <spatial/desktop/MessageBox.h>
#include <spatial/ui/UserInterfaceSystem.h>

#include <stdexcept>

namespace spatial
{

int setup(const SetupConfig& config, std::function<int(Application&, SystemServices&)>&& action)
{
	try
	{
		auto app = Application{};
		auto window = app.getWindowContext().createWindow(config.windowWidth, config.windowHeight, config.windowTitle);
		auto input = System<InputSystem>{app, window};
		auto rendering = System<RenderingSystem>{app, window};
		auto ui = System<UserInterfaceSystem>(app, rendering.get(), window, config.uiFontResourceId);
		auto services = SystemServices{window, rendering.get(), input.get(), ui.get()};

		return action(app, services);
	}
	catch (const std::exception& e)
	{
		showMessageBox(MessageBoxType::Error, "[Spatial Engine] Something went wrong", e.what());
		return -1;
	}
}

SetupConfig SetupConfig::fromConfig(const Configuration& config)
{
	auto fontPath = config.get<std::string>("ui.font-path", "");

	return {
		config.get<std::string>("window.title", "Spatial Engine"),
		config.get<int>("window.width", 1280),
		config.get<int>("window.height", 720),
		hash<uint32_t>(fontPath)
	};
}

} // namespace spatial