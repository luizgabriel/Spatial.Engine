#include "MaterialInputs.h"
#include "Icons.h"
#include "spatial/ecs/Texture.h"
#include "spatial/graphics/TextureUtils.h"
#include "spatial/ui/components/Basic.h"
#include "spatial/ui/components/DragAndDrop.h"
#include "spatial/ui/components/PreviewTexture.h"
#include <imgui.h>

namespace spatial::ui
{

bool albedoInput(std::string_view label, math::vec3& color, ecs::Registry& registry, ecs::Entity& resource,
				 graphics::OptionalTexture icons)
{
	ImGui::PushID(label.data());
	ImGui::Columns(2);

	constexpr auto size = math::vec2{20.0f};

	ui::previewTexture(registry, resource, icons, Icons::picture.uv());

	ImGui::SameLine();
	ui::spanToAvailWidth();

	bool changed = ui::Search::searchResource<ecs::tags::IsImageTexture>("##Path", icons, registry, resource);

	ImGui::NextColumn();
	ui::image(icons, size, Icons::picker.uv());
	ImGui::SameLine();
	changed = changed || ImGui::ColorEdit3(label.data(), &color.r, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoInputs);
	ImGui::Columns(1);
	ImGui::PopID();

	return changed;
}

bool mapInput(std::string_view label, ecs::Registry& registry, ecs::Entity& resource, graphics::OptionalTexture icons,
			  math::vec4 uv)
{
	ui::previewTexture(registry, resource, icons, uv);

	ImGui::SameLine();

	return ui::Search::searchResource<ecs::tags::IsImageTexture>(label.data(), icons, registry, resource);
}

bool mapInput(std::string_view label, float& value, ecs::Registry& registry, ecs::Entity& resource,
			  graphics::OptionalTexture icons, math::vec4 uv)
{
	ImGui::PushID(label.data());
	ImGui::Columns(2);

	ui::previewTexture(registry, resource, icons, uv);

	ImGui::SameLine();
	ui::spanToAvailWidth();

	bool changed = ui::Search::searchResource<ecs::tags::IsImageTexture>("##Path", icons, registry, resource);

	ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetColumnWidth() * 0.4f);
	changed = changed || ImGui::SliderFloat(label.data(), &value, .0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::Columns(1);
	ImGui::PopID();

	return changed;
}

bool colorPicker(std::string_view label, math::vec4& color, graphics::OptionalTexture icons)
{
	static constexpr auto size = math::vec2{20.0F};

	ui::image(icons, size, Icons::picker.uv());
	ImGui::SameLine();

	return ImGui::ColorEdit4(label.data(), &color.r,
							 ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoInputs
								 | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
}

bool colorPicker(std::string_view label, math::vec3& color, graphics::OptionalTexture icons)
{
	static constexpr auto size = math::vec2{20.0F};

	ui::image(std::move(icons), size, Icons::picker.uv());
	ImGui::SameLine();

	return ImGui::ColorEdit3(label.data(), &color.r, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoInputs);
}

bool cubemapInput(std::string_view label, ecs::Registry& registry, ecs::Entity& resource,
				  graphics::OptionalTexture icons)
{
	static constexpr auto size = math::vec2{20.0F};

	ui::image(icons, size, Icons::cubemap.uv());

	ImGui::SameLine();

	return ui::Search::searchResource<ecs::tags::IsCubeMapTexture>(label.data(), std::move(icons), registry, resource);
}

} // namespace spatial::ui