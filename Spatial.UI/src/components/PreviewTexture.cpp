#include <imgui.h>
#include <spatial/graphics/TextureUtils.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/PreviewTexture.h>

namespace spatial::ui
{

void previewTexture(const ecs::Registry& registry, ecs::Entity resourceEntity, const filament::Texture* icons,
					const math::vec4 uv)
{
	constexpr auto size = math::vec2{20.0f};
	const auto* texture = graphics::getTexture(registry, resourceEntity);

	ui::image(texture, size);

	if (texture == nullptr)
		ui::image(icons, size, uv);

	const auto& colors = ImGui::GetStyle().Colors;

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();

		if (texture)
		{
			ImGui::Text("Preview");
			ui::image(texture, math::vec2{300.0f});
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