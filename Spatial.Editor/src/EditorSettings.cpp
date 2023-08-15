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
	auto dim = math::uvec2{};
	args({"-w", "--width"}) >> dim.x;
	args({"-h", "--height"}) >> dim.y;

	if (dim.x > 0 && dim.y > 0)
		config.windowDimensions = dim;

	auto projectFolder = fs::path{args[1]};
	if (fs::is_directory(projectFolder))
		config.projectFolder = std::move(projectFolder);

	return config;
}

} // namespace spatial::editor