#pragma once

#include <filesystem>
#include <string_view>
#include <spatial/common/Math.h>
#include <spatial/common/AspectRatio.h>

namespace spatial::editor
{

struct Settings
{
	std::string windowTitle{"Spatial Engine | Editor"};
	std::optional<math::uvec2> windowDimensions{std::nullopt};
	std::optional<std::filesystem::path> projectFolder{std::nullopt};

	static Settings load(int argc, char* argv[]);
};

} // namespace spatial::editor
