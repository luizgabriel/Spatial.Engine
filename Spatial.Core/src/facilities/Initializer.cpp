#include <spatial/facilities/Initializer.h>
#include <spatial/core/System.h>
#include <spatial/input/InputSystem.h>
#include <spatial/desktop/MessageBox.h>

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
		auto rendering = System<RenderingSystem>{app, fl::backend::Backend::OPENGL, window};
		auto services = SystemServices{window, rendering.get(), input.get()};

		return action(app, services);
	}
	catch (const std::exception& e)
	{
		showMessageBox(MessageBoxType::Error, "[Spatial Engine] Something went wrong", e.what());
		return -1;
	}
}

} // namespace spatial