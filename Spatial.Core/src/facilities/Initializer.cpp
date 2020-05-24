#include <spatial/facilities/Initializer.h>
#include <spatial/core/System.h>
#include <spatial/input/InputSystem.h>
#include <spatial/desktop/MessageBox.h>

#include <stdexcept>

namespace spatial
{

Window createDefaultWindow(Application& app, const Configuration& config)
{
	const auto width = config.get<int>("window.width");
	const auto height = config.get<int>("window.height");
	const auto title = config.get<std::string>("window.title");

	auto window = app.getWindowContext().createWindow(width, height, title);

	return window;
}

int start(const Configuration& config, const std::function<void(Application&, RenderingSystem&)>& action)
{
	try
	{
		auto app = Application{};
		auto window = createDefaultWindow(app, config);
		auto rendering = System<RenderingSystem>{app, &window};
		auto input = System<InputSystem>{app, &window};

		action(app, rendering.get());

		return app.run();
	}
	catch (const std::exception& e)
	{
		showMessageBox(MessageBoxType::Error, "[Spatial Engine] Something went wrong", e.what());
		return -1;
	}
}

} // namespace spatial