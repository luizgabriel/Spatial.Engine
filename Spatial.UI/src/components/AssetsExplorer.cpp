#include <boost/algorithm/string/predicate.hpp>
#include <imgui.h>
#include <spatial/ui/components/AssetsExplorer.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/Icons.h>

namespace spatial::ui
{

bool AssetsExplorer::displayFiles(FileSystem& fileSystem, std::filesystem::path& selectedPath,
								  const filament::Texture& icon)
{
	using namespace boost::algorithm;
	using namespace std::filesystem;

	displayPathHeader(selectedPath, icon);

	ImGui::Columns(std::max(4, static_cast<int>(ImGui::GetContentRegionAvail().x) / 70), "AssetsExplorer", false);

	bool selected = false;
	const auto size = math::float2{std::clamp(ImGui::GetContentRegionAvail().x * 0.9f, 30.0f, 50.0f)};
	for (const auto& entry : fileSystem.list(selectedPath.string()))
	{
		ImGui::SetCursorPosX(ImGui::GetColumnOffset() + (ImGui::GetColumnWidth() - size.x) * 0.5f - 5.0f);

		if (entry.isDirectory())
		{
			ImGui::PushID(entry.path.c_str());
			if (imageButton(icon, size, Icons::folder.uv()))
			{
				selectedPath = selectedPath / entry.path;
				selected = true;
			}
			ImGui::PopID();
		}

		else if (entry.isFile())
		{
			if (starts_with(entry.path, "."))
				continue;

			const auto fileButton = [&](const math::float4& uv) {
				ImGui::PushID(entry.path.c_str());
				if (imageButton(icon, size, uv))
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
					dnd.setPayload(selectedPath / entry.path);
			}
		}

		ImGui::SetCursorPosX(ImGui::GetColumnOffset() + (ImGui::GetColumnWidth() - size.x) * 0.5f - 4.0f);
		ImGui::TextWrapped("%s", entry.path.c_str());
		ImGui::NextColumn();
	}

	ImGui::Columns(1);

	return selected;
}

bool AssetsExplorer::displayPathHeader(std::filesystem::path& selectedPath, const filament::Texture& icon)
{
	bool changed = false;
	if (selectedPath.empty())
		return false;

	ImGui::PushID("BackButton");
	changed = imageButton(icon, math::float2{20}, Icons::back.uv());
	if (changed)
		selectedPath = selectedPath.parent_path();
	ImGui::PopID();

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::TextWrapped("%s", selectedPath.c_str());
	ImGui::Separator();

	return changed;
}

} // namespace spatial::ui