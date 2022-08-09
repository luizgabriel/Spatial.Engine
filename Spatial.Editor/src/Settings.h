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
	math::uvec2 windowDimensions{gWideScreenAspectRatio.toVector() * 80.0};
	std::filesystem::path projectFolder{};

	static Settings load(int argc, char* argv[]);
};

} // namespace spatial::editor
