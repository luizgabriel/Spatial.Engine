#include <spatial/ui/components/ResourceSelector.h>

namespace spatial::ui
{

void imageTextureInput(const std::string_view label, Resource<ResourceType::ImageTexture>& resource, const filament::Texture* texture, const filament::Texture& icons, std::string_view dndTargetId)
{
	spacing(1);
	ImGui::Columns(2, label.data(), false);

	ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() * .15f);

	const auto previewSize = math::float2{std::clamp(ImGui::GetContentRegionAvailWidth(), 50.0f, 100.0f)};

	if (texture != nullptr)
	{
		image(*texture, previewSize);
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Preview");
			ui::image(*texture, math::float2{300.0f});
			ImGui::EndTooltip();
		}
	}
	else
	{
		image(icons, previewSize, Icons::pngFile.uv());
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("No Image Selected");
			ImGui::EndTooltip();
		}
	}

	ImGui::NextColumn();

	ImGui::PushID(label.data());
	ImGui::Text("%s", label.data());
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	inputPath("##Resource", resource.relativePath);
	auto dnd = DragAndDropTarget{};
	if (dnd.isStarted())
	{
		auto result = dnd.getPathPayload(dndTargetId);
		if (result)
			resource.relativePath = std::move(result.value());
	}
	ImGui::PopID();

	ImGui::Columns(1);
	spacing(1);
}

void cubemapTextureInput(const std::string_view label, Resource<ResourceType::CubeMapTexture>& resource,
						 const filament::Texture*, const filament::Texture& icons, std::string_view dndTargetId)
{
	spacing(1);
	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() * .15f);

	const auto previewSize = math::float2{std::clamp(ImGui::GetContentRegionAvailWidth(), 50.0f, 100.0f)};
	image(icons, previewSize, Icons::ktxFile.uv());

	ImGui::NextColumn();

	ImGui::PushID(label.data());
	ImGui::Text("%s", label.data());
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	inputPath("##Resource", resource.relativePath);
	auto dnd = DragAndDropTarget{};
	if (dnd.isStarted())
	{
		auto result = dnd.getPathPayload(dndTargetId);
		if (result)
			resource.relativePath = std::move(result.value());
	}
	ImGui::PopID();

	ImGui::Columns(1);
	spacing(1);
}

} // namespace spatial::ui