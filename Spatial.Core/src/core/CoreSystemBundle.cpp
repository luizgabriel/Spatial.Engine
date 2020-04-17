#include <spatial/core/CoreSystemBundle.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace spatial
{

CoreSystemBundle::CoreSystemBundle(Application& app, const toml::table& config)
	: window{[&app, &config]()
	  {
		  const auto windowTitle = config["window"]["title"].as_string()->get();
		  const auto windowWidth = config["window"]["width"].as_integer()->get();
		  const auto windowHeight = config["window"]["height"].as_integer()->get();

		  return app.getWindowContext().createWindow(windowWidth, windowHeight, windowTitle);
	  }()},
	  input{app},
	  rendering{app, window.getNativeHandle()},
	  ui{app, rendering.get(), fs::path{"fonts"} / "Roboto-Medium.ttf"}
{
	rendering.get().setupViewport(window.getFrameBufferSize());
	ui.get().setupViewport(window.getWindowSize(), window.getFrameBufferSize());
}

}
