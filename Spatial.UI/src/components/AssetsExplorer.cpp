#include <boost/algorithm/string/predicate.hpp>
#include <imgui.h>
#include <spatial/ui/components/AssetsExplorer.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/Icons.h>

namespace spatial::ui
{

bool AssetsExplorer::displayFiles(const std::filesystem::path& rootPath, std::filesystem::path& selectedPath,
								  const filament::Texture& icon)
{
	using namespace boost::algorithm;
	using namespace std::filesystem;

	if (!is_directory(rootPath / selectedPath))
		return false;

	displayPathHeader(rootPath, selectedPath, icon);

	ImGui::Columns(6, "AssetsExplorer", false);

	bool selected = false;
	const auto size = math::float2{std::clamp(ImGui::GetContentRegionAvail().x * 0.9f, 30.0f, 50.0f)};
	for (const auto& entry : directory_iterator{rootPath / selectedPath})
	{
		const auto& path = entry.path();

		ImGui::SetCursorPosX(ImGui::GetColumnOffset() + (ImGui::GetColumnWidth() - size.x) * 0.5f - 5.0f);

		if (entry.is_directory())
		{
			ImGui::PushID(path.c_str());
			if (imageButton(icon, size, Icons::folder.uv()))
			{
				selectedPath = path;
				selected = true;
			}
			ImGui::PopID();
		}

		if (entry.is_regular_file())
		{
			const auto filename = entry.path().filename().string();
			if (starts_with(filename, "."))
				continue;

			const auto fileButton = [&](const math::float4& uv) {
				ImGui::PushID(filename.data());
				if (imageButton(icon, size, uv))
					selected = true;
				ImGui::PopID();
			};

			if (ends_with(filename, ".png"))
				fileButton(Icons::pngFile.uv());
			else if (ends_with(filename, ".jpg"))
				fileButton(Icons::jpgFile.uv());
			else if (ends_with(filename, ".js"))
				fileButton(Icons::jsFile.uv());
			else if (ends_with(filename, ".exr"))
				fileButton(Icons::exrFile.uv());
			else if (ends_with(filename, ".ktx"))
				fileButton(Icons::ktxFile.uv());
			else if (ends_with(filename, ".spatial.json"))
				fileButton(Icons::sceneFile.uv());
			else if (ends_with(filename, ".filamesh") || ends_with(filename, ".obj"))
				fileButton(Icons::meshFile.uv());
			else
				fileButton(Icons::unknownFile.uv());

			{
				auto dnd = DragAndDropSource{};
				if (dnd.isStarted()) {
					dnd.setPayload(std::filesystem::relative(path, rootPath));
				}
			}
		}

		ImGui::SetCursorPosX(ImGui::GetColumnOffset() + (ImGui::GetColumnWidth() - size.x) * 0.5f - 4.0f);
		ImGui::TextWrapped("%s", path.filename().c_str());
		ImGui::NextColumn();
	}

	ImGui::Columns(1);

	return selected;
}

bool AssetsExplorer::displayPathHeader(const std::filesystem::path& rootPath, std::filesystem::path& selectedPath,
									   const filament::Texture& icon)
{
	bool changed = false;
	auto fullPath = (rootPath / selectedPath).lexically_normal();

	if (fullPath != rootPath)
	{
		ImGui::PushID("BackButton");
		changed = imageButton(icon, math::float2{20}, Icons::back.uv());
		if (changed)
			selectedPath = selectedPath.parent_path();
		ImGui::PopID();
	}

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::TextWrapped("%s", (rootPath / selectedPath).string().c_str());
	ImGui::Separator();

	return changed;
}

} // namespace spatial::ui