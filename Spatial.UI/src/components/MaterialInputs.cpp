#include <imgui.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/Icons.h>
#include <spatial/ui/components/MaterialInputs.h>

namespace spatial::ui
{

bool albedoInput(std::string_view label, math::float3& color, Resource<ImageTexture>& resource,
				 const filament::Texture* texture, const filament::Texture& icons, std::string_view dndTargetId)
{
	ImGui::PushID(label.data());
	ImGui::Columns(2);

	bool changed = false;

	constexpr auto size = math::float2{20.0f};

	ui::previewTexture(texture, icons, Icons::picture.uv());

	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	static const char* placeholder = "resource/path.png";

	changed |= ui::inputPath("##Path", resource.relativePath, placeholder);

	auto dndTarget = ui::DragAndDropTarget{};
	if (dndTarget.isStarted())
	{
		auto result = dndTarget.getPathPayload(dndTargetId);
		if (result)
		{
			resource.relativePath = std::move(result.value());
			changed = true;
		}
	}

	ImGui::NextColumn();
	ui::image(icons, size, Icons::picker.uv());
	ImGui::SameLine();
	changed |= ImGui::ColorEdit3(label.data(), &color.r, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoInputs);
	ImGui::Columns(1);
	ImGui::PopID();

	return changed;
}

bool mapInput(std::string_view label, Resource<ImageTexture>& resource, const filament::Texture* texture,
			  const filament::Texture& icons, const math::float4& uv, std::string_view dndTargetId)
{

	bool changed = false;

	ui::previewTexture(texture, icons, uv);

	ImGui::SameLine();
	static const char* placeholder = "resource/path.png";

	changed |= ui::inputPath(label.data(), resource.relativePath, placeholder);

	auto dndTarget = ui::DragAndDropTarget{};
	if (dndTarget.isStarted())
	{
		auto result = dndTarget.getPathPayload(dndTargetId);
		if (result)
		{
			resource.relativePath = std::move(result.value());
			changed = true;
		}
	}

	return changed;
}

bool mapInput(std::string_view label, float& value, Resource<ImageTexture>& resource, const filament::Texture* texture,
			  const filament::Texture& icons, const math::float4& uv, std::string_view dndTargetId)
{
	ImGui::PushID(label.data());
	ImGui::Columns(2);

	bool changed = false;

	ui::previewTexture(texture, icons, uv);

	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	static const char* placeholder = "resource/path.png";

	changed |= ui::inputPath("##Path", resource.relativePath, placeholder);

	auto dndTarget = ui::DragAndDropTarget{};
	if (dndTarget.isStarted())
	{
		auto result = dndTarget.getPathPayload(dndTargetId);
		if (result)
		{
			resource.relativePath = std::move(result.value());
			changed = true;
		}
	}

	ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetColumnWidth() * 0.4f);
	changed |= ImGui::SliderFloat(label.data(), &value, .0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::Columns(1);
	ImGui::PopID();

	return changed;
}

bool cubemapInput(std::string_view label, math::float4& color, Resource<CubeMapTexture>& resource,
				  const filament::Texture& icons, std::string_view dndTargetId)
{
	ImGui::PushID(label.data());
	ImGui::Columns(2);

	bool changed = false;

	constexpr auto size = math::float2{20.0f};

	ui::image(icons, size, Icons::cubemap.uv());

	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	static const char* placeholder = "resource/path.ktx";

	changed |= ui::inputPath("##Path", resource.relativePath, placeholder);

	auto dndTarget = ui::DragAndDropTarget{};
	if (dndTarget.isStarted())
	{
		auto result = dndTarget.getPathPayload(dndTargetId);
		if (result)
		{
			resource.relativePath = std::move(result.value());
			changed = true;
		}
	}

	ImGui::NextColumn();
	ui::image(icons, size, Icons::picker.uv());
	ImGui::SameLine();
	changed |= ImGui::ColorEdit4(label.data(), &color.r, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
	ImGui::Columns(1);
	ImGui::PopID();

	return changed;
}

void previewTexture(const filament::Texture* texture, const filament::Texture& icons, const math::float4& uv)
{
	constexpr auto size = math::float2{20.0f};

	if (texture != nullptr)
		ui::image(*texture, size);
	else
		ui::image(icons, size, uv);

	const auto& colors = ImGui::GetStyle().Colors;

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();

		if (texture)
		{
			ImGui::Text("Preview");
			ui::image(*texture, math::float2{300.0f});
		}
		else
		{
			ImGui::Text("No Texture Attached.");
			ImGui::TextColored(colors[ImGuiCol_TextSelectedBg], "Drag and drop image assets to fill");
		}

		ImGui::EndTooltip();
	}
}

} // namespace spatial::ui