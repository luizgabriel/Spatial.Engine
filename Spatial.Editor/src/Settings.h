#pragma once

#include <filesystem>
#include <spatial/ecs/Registry.h>

namespace spatial::editor
{

struct Settings
{
	std::string_view windowTitle;
	std::uint16_t windowWidth;
	std::uint16_t windowHeight;
	std::filesystem::path projectFolder;

	static Settings load(int argc, char* argv[]);
};

} // namespace spatial::editor
