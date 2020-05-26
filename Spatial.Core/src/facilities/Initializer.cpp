#include <spatial/facilities/Initializer.h>
#include <spatial/core/System.h>
#include <spatial/input/InputSystem.h>
#include <spatial/desktop/MessageBox.h>
#include <spatial/ui/UserInterfaceSystem.h>

#include <stdexcept>

namespace spatial
{

Window createDefaultWindow(const Application& app, const Configuration& config)
{
	const auto width = config.get<int>("window.width");
	const auto height = config.get<int>("window.height");
	const auto title = config.get<std::string>("window.title");

	auto window = app.getWindowContext().createWindow(width, height, title);

	return window;
}

int setup(const Configuration& config, std::function<int(Application&, SystemServices&)>&& action)
{
	try
	{
		auto app = Application{};
		auto window = createDefaultWindow(app, config);
		auto input = System<InputSystem>{app, &window};
		auto rendering = System<RenderingSystem>{app, window};

		auto fontPath = config.get<std::string>("ui.font-path");
		auto ui = System<UserInterfaceSystem>(app, rendering.get(), window, fontPath);

		return action(app, SystemServices{window, rendering.get(), input.get(), ui.get()});
	}
	catch (const std::exception& e)
	{
		showMessageBox(MessageBoxType::Error, "[Spatial Engine] Something went wrong", e.what());
		return -1;
	}
}

} // namespace spatial