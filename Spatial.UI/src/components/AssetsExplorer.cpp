#include <boost/algorithm/string/predicate.hpp>
#include <imgui.h>
#include <spatial/ui/components/AssetsExplorer.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/Icons.h>
#include <spatial/ui/components/DragAndDrop.h>

namespace spatial::ui
{

AssetsExplorer::AssetsExplorer(const std::filesystem::path& rootPath, const filament::Texture& texture)
	: mWindow{"Assets Explorer"}, mRootPath{rootPath}, mIconTexture{texture}
{
}

bool AssetsExplorer::header(std::filesystem::path& selectedPath)
{
	bool changed = false;
	auto fullPath = (mRootPath / selectedPath).lexically_normal();

	if (fullPath != mRootPath)
	{
		ImGui::PushID("BackButton");
		changed = imageButton(mIconTexture, math::float2{20}, gIcons.back.uv());
		if (changed)
			selectedPath = selectedPath.parent_path();
		ImGui::PopID();
	}

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::TextWrapped("%s", (mRootPath / selectedPath).string().c_str());
	ImGui::Separator();

	ImGui::Columns(8);
	return changed;
}

AssetsExplorer::~AssetsExplorer()
{
	ImGui::Columns(1);
}

bool AssetsExplorer::onSelectPath(std::filesystem::path& selectedPath)
{
	using namespace boost::algorithm;
	using namespace std::filesystem;

	if (!is_directory(mRootPath / selectedPath))
		return false;

	bool selected = false;
	const auto size = math::float2{std::clamp(ImGui::GetContentRegionAvailWidth() * 0.9f, 50.0f, 100.0f)};
	for (const auto& entry : directory_iterator{mRootPath / selectedPath})
	{
		const auto& path = entry.path();

		if (entry.is_directory())
		{
			ImGui::PushID(path.c_str());
			if (imageButton(mIconTexture, size, gIcons.folder.uv()))
			{
				selectedPath = path;
				selected = true;
			}
			ImGui::PopID();
		}

		if (entry.is_regular_file())
		{
			const auto filename = entry.path().filename().string();
			if (starts_with(filename, ".")) continue;

			const auto fileButton = [&, this](const math::float4& uv) {
				ImGui::PushID(filename.data());
				if (imageButton(mIconTexture, size, uv))
					selected = true;
				ImGui::PopID();
			};

			if (ends_with(filename, ".png"))
				fileButton(gIcons.pngFile.uv());
			else if (ends_with(filename, ".jpg"))
				fileButton(gIcons.jpgFile.uv());
			else if (ends_with(filename, ".js"))
				fileButton(gIcons.jsFile.uv());
			else if (ends_with(filename, ".exr"))
				fileButton(gIcons.exrFile.uv());
			else if (ends_with(filename, ".spatial.xml"))
				fileButton(gIcons.sceneFile.uv());
			else if (ends_with(filename, ".filamesh") || ends_with(filename, ".obj"))
				fileButton(gIcons.meshFile.uv());
			else
				fileButton(gIcons.unknownFile.uv());

			{
				auto dnd = DragAndDropSource{};
				if (dnd.isStarted()) {
					dnd.setPayload(DND_SELECTED_FILE, std::filesystem::relative(path, mRootPath));
				}
			}
		}

		ImGui::TextWrapped("%s", path.filename().c_str());
		ImGui::NextColumn();
	}

	return selected;
}

} // namespace spatial::ui