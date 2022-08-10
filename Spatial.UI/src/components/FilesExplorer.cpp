#include <boost/algorithm/string/predicate.hpp>
#include <imgui.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/FilesExplorer.h>
#include <spatial/ui/components/Icons.h>

namespace spatial::ui
{

bool FilesExplorer::displayFiles(FileSystem& fileSystem, std::string& selectedPath, graphics::OptionalTexture icons)
{
	using namespace boost::algorithm;
	using namespace std::filesystem;

	displayPathHeader(selectedPath, icons);

	ImGui::Columns(std::max(4, static_cast<int>(ImGui::GetContentRegionAvail().x) / 70), "FilesExplorer", false);

	bool selected = false;
	const auto size = math::vec2{std::clamp(ImGui::GetContentRegionAvail().x * 0.9f, 30.0f, 50.0f)};
	for (const auto& entry : fileSystem.list(selectedPath))
	{
		ImGui::SetCursorPosX(ImGui::GetColumnOffset() + (ImGui::GetColumnWidth() - size.x) * 0.5f - 5.0f);
		auto newPath = selectedPath.empty() ? entry.path : selectedPath + FileSystem::SEPARATOR + entry.path;

		if (entry.isDirectory())
		{
			ImGui::PushID(entry.path.c_str());
			if (imageButton(icons, size, Icons::folder.uv()))
			{
				selectedPath = newPath;
				selected = true;
			}
			ImGui::PopID();
		}

		else if (entry.isFile())
		{
			if (starts_with(entry.path, "."))
				continue;

			const auto fileButton = [&](const math::vec4& uv) {
				ImGui::PushID(entry.path.c_str());
				if (imageButton(icons, size, uv))
					selected = true;
				ImGui::PopID();
			};

			if (ends_with(entry.path, ".png"))
				fileButton(Icons::pngFile.uv());
			else if (ends_with(entry.path, ".jpg"))
				fileButton(Icons::jpgFile.uv());
			else if (ends_with(entry.path, ".js"))
				fileButton(Icons::jsFile.uv());
			else if (ends_with(entry.path, ".exr"))
				fileButton(Icons::exrFile.uv());
			else if (ends_with(entry.path, ".ktx"))
				fileButton(Icons::ktxFile.uv());
			else if (ends_with(entry.path, ".spatial.json"))
				fileButton(Icons::sceneFile.uv());
			else if (ends_with(entry.path, ".filamesh") || ends_with(entry.path, ".obj"))
				fileButton(Icons::meshFile.uv());
			else
				fileButton(Icons::unknownFile.uv());

			{
				auto dnd = DragAndDropSource{};
				if (dnd.isStarted())
					dnd.setPayload(newPath);
			}
		}

		ImGui::SetCursorPosX(ImGui::GetColumnOffset() + (ImGui::GetColumnWidth() - size.x) * 0.5f - 4.0f);
		ImGui::TextWrapped("%s", entry.path.c_str());
		ImGui::NextColumn();
	}

	ImGui::Columns(1);

	return selected;
}

bool FilesExplorer::displayPathHeader(std::string& selectedPath, graphics::OptionalTexture icons)
{
	bool changed = false;
	if (selectedPath.empty())
		return false;

	ImGui::PushID("BackButton");
	changed = imageButton(icons, math::vec2{20}, Icons::back.uv());
	if (changed)
	{
		auto lastSeparator = selectedPath.find_last_of(FileSystem::SEPARATOR);
		if (lastSeparator == std::string::npos)
			selectedPath = "";
		else
			selectedPath = selectedPath.substr(0, lastSeparator);
	}

	ImGui::PopID();

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("%s", selectedPath.c_str());
	ImGui::Separator();

	return changed;
}

} // namespace spatial::ui