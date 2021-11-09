#include "Settings.h"

#include <fstream>
#include <magic_enum.hpp>

namespace fs = std::filesystem;

namespace spatial::editor
{

Settings Settings::load(int argc, char** argv)
{
	const auto args = argh::parser(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);
	const auto executablePath = fs::path{args[0]}.parent_path();

	auto config = Settings{"Spatial Engine | Editor", 1280, 720, ""};
	args({"-w", "--width"}) >> config.windowWidth;
	args({"-h", "--height"}) >> config.windowHeight;

	std::string assetsFolder = "assets";
	args({"-a", "--assets"}) >> assetsFolder;
	config.assetsFolder = fs::is_directory(assetsFolder)  ? std::filesystem::path{assetsFolder} : std::filesystem::current_path();

	return config;
}

} // namespace spatial::editor