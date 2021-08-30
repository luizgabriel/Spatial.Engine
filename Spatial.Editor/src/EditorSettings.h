#pragma once

#include <argh.h>
#include <ghc/filesystem.hpp>

namespace spatial::editor
{

struct EditorSettings
{
	std::string_view windowTitle;
	std::uint16_t windowWidth;
	std::uint16_t windowHeight;
};

EditorSettings loadEditorSettings(int argc, char* argv[]);

} // namespace spatial::editor
