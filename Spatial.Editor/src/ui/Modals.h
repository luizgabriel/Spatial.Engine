#pragma once

namespace spatial::ui
{

struct EditorModals
{
	static bool newScene();
	static bool openScene(std::string& openPath);
	static bool saveScene(std::string& savePath);
	static bool openProject(std::string& openPath);
};

} // namespace spatial::ui