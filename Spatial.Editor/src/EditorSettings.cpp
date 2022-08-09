#include "Settings.h"

#include <argh.h>

namespace fs = std::filesystem;

namespace spatial::editor
{

Settings Settings::load(int argc, char** argv)
{
	const auto args = argh::parser(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);
	const auto executablePath = fs::path{args[0]}.parent_path();

	auto config = Settings{};
	args({"-w", "--width"}) >> config.windowDimensions.x;
	args({"-h", "--height"}) >> config.windowDimensions.y;

	std::filesystem::path projectFolder = args[1];
	config.projectFolder = fs::is_directory(projectFolder) ? projectFolder : "";

	return config;
}

} // namespace spatial::editor