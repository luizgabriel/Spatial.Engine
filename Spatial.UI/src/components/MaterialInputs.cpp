#include <imgui.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/Icons.h>
#include <spatial/ui/components/MaterialInputs.h>

namespace spatial::ui
{

bool albedoInput(std::string_view label, math::float3& color, std::string& resource,
				 const filament::Texture* texture, const filament::Texture& icons)
{
	ImGui::PushID(label.data());
	ImGui::Columns(2);

	bool changed = false;

	constexpr auto size = math::float2{20.0f};

	ui::previewTexture(texture, icons, Icons::picture.uv());

	ImGui::SameLine();
	ui::spanToAvailWidth();
	static const char* placeholder = "resource/path.png";

	changed |= ui::inputPath("##Path", resource, placeholder);

	ImGui::NextColumn();
	ui::image(icons, size, Icons::picker.uv());
	ImGui::SameLine();
	changed |= ImGui::ColorEdit3(label.data(), &color.r, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoInputs);
	ImGui::Columns(1);
	ImGui::PopID();

	return changed;
}

bool mapInput(std::string_view label, std::string& resource, const filament::Texture* texture,
			  const filament::Texture& icons, const math::float4& uv)
{

	bool changed = false;

	ui::previewTexture(texture, icons, uv);

	ImGui::SameLine();
	static const char* placeholder = "resource/path.png";

	changed |= ui::inputPath(label.data(), resource, placeholder);

	return changed;
}

bool mapInput(std::string_view label, float& value, std::string& resource, const filament::Texture* texture,
			  const filament::Texture& icons, const math::float4& uv)
{
	ImGui::PushID(label.data());
	ImGui::Columns(2);

	bool changed = false;

	ui::previewTexture(texture, icons, uv);

	ImGui::SameLine();
	ui::spanToAvailWidth();
	static const char* placeholder = "resource/path.png";

	changed |= ui::inputPath("##Path", resource, placeholder);

	ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetColumnWidth() * 0.4f);
	changed |= ImGui::SliderFloat(label.data(), &value, .0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::Columns(1);
	ImGui::PopID();

	return changed;
}

bool colorPicker(std::string_view label, math::float4& color, const filament::Texture& icons)
{
	static constexpr auto size = math::float2{20.0f};

	bool changed = false;

	ui::image(icons, size, Icons::picker.uv());
	ImGui::SameLine();
	changed |= ImGui::ColorEdit4(label.data(), &color.r,
								 ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoInputs
									 | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

	return changed;
}

bool colorPicker(std::string_view label, math::float3& color, const filament::Texture& icons)
{
	static constexpr auto size = math::float2{20.0f};

	bool changed = false;

	ui::image(icons, size, Icons::picker.uv());
	ImGui::SameLine();
	changed |= ImGui::ColorEdit3(label.data(), &color.r,
								 ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoInputs);

	return changed;
}

bool cubemapInput(std::string_view label, std::string& resource,
				  const filament::Texture& icons)
{
	static constexpr auto size = math::float2{20.0f};

	bool changed = false;

	ui::image(icons, size, Icons::cubemap.uv());

	ImGui::SameLine();
	static const char* placeholder = "resource/path.ktx";

	changed |= ui::inputPath(label.data(), resource, placeholder);

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

void inputHelp(std::string_view title, std::string_view description)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		const auto& colors = ImGui::GetStyle().Colors;

		ImGui::Text("%s", title.data());
		ImGui::PushStyleColor(ImGuiCol_Text, colors[ImGuiCol_ButtonActive]);

		ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 300.0f);
		ImGui::TextWrapped("%s", description.data());
		ImGui::PopStyleColor();
		ImGui::EndTooltip();
	}
}

} // namespace spatial::ui