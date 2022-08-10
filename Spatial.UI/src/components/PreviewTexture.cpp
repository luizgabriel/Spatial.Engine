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
	auto isDepthTexture = texture != nullptr && isDepthFormat(texture->getFormat());

	if (texture == nullptr || isDepthTexture)
		ui::image(icons, size, uv);

	if (texture != nullptr && !isDepthTexture)
		ui::image(texture, size);

	const auto& colors = ImGui::GetStyle().Colors;

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();

		if (texture)
		{
			ImGui::Text("Preview");
			ui::separator();
			if (isDepthFormat(texture->getFormat()))
				ImGui::Text("Depth Textures cannot be previewed");
			else
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