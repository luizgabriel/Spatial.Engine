#include "Settings.h"

#include <fstream>
#include <argh.h>

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

	std::filesystem::path projectFolder = args[1];
	config.projectFolder = fs::is_directory(projectFolder) ? projectFolder : std::filesystem::current_path();

	return config;
}

} // namespace spatial::editor