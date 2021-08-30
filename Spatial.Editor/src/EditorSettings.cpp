#include "EditorSettings.h"

namespace fs = ghc::filesystem;

namespace spatial::editor
{

EditorSettings loadEditorSettings(int argc, char* argv[])
{
	const auto args = argh::parser(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);
	const auto executablePath = fs::path{args[0]}.parent_path();

	auto config = EditorSettings{"Spatial Engine | Editor", 1280, 720};
	args({"-w", "--width"}) >> config.windowWidth;
	args({"-h", "--height"}) >> config.windowHeight;

	return config;
}

} // namespace spatial::editor