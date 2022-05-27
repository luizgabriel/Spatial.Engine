#include <imgui.h>
#include <imgui_internal.h>
#include <spatial/ecs/SceneView.h>
#include <spatial/graphics/TextureView.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/SceneView.h>

namespace spatial::ui
{

void SceneView::image(const ecs::Registry& registry, ecs::Entity sceneViewEntity, math::vec2 size)
{
	{
		const auto currentPosition = ImGui::GetCursorScreenPos();
		const auto rectMax = ImVec2(currentPosition.x + size.x, currentPosition.y + size.y);

		ImGui::GetWindowDrawList()->AddRectFilled(currentPosition, rectMax, IM_COL32(34, 34, 34, 255));
	}

	if (!registry.hasAllComponents<ecs::SceneView, graphics::TextureView>(sceneViewEntity))
		return;

	const auto& sceneView = registry.getComponent<const ecs::SceneView>(sceneViewEntity);
	const auto& textureView = registry.getComponent<const graphics::TextureView>(sceneViewEntity);

	double aspectRatio = 1.0;
	const auto* perspectiveCamera = registry.tryGetComponent<const ecs::PerspectiveCamera>(sceneView.camera);
	if (perspectiveCamera)
		aspectRatio = perspectiveCamera->aspectRatio;

	const auto imageSize = aspectRatio >= 1 ? math::uvec2{static_cast<double>(size.y) * aspectRatio, size.y}
											: math::uvec2{size.x, static_cast<double>(size.x) / aspectRatio};

	ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x + (size.x - static_cast<float>(imageSize.x)) * 0.5f,
									 ImGui::GetCursorScreenPos().y));
	ui::image(textureView.getColorTexture().get(), imageSize, math::vec4{0, 1, 1, 0});
}

bool SceneView::selector(const ecs::Registry& registry, ecs::Entity& sceneViewEntity)
{
	auto windowWidth = ImGui::GetContentRegionAvail().x;
	auto sceneViewName = registry.tryGetComponent<ecs::Name>(sceneViewEntity);
	bool changed = false;

	auto cursorPosition = ImGui::GetCursorPos();
	constexpr auto comboWidth = 200;

	auto view = registry.getEntities<const ecs::Name, const ecs::SceneView>();

	ImGui::SetCursorPos(ImVec2(windowWidth - comboWidth - 10, 30));

	if (view.size_hint() <= 1)
		return false;

	{
		ImGui::SetNextItemWidth(comboWidth);
		auto combo = ui::Combo{"##SceneViewSelector", sceneViewName ? sceneViewName->name : "No View Selected"};
		if (combo.isOpen())
		{
			registry.getEntities<const ecs::Name, const ecs::SceneView>().each(
				[&](ecs::Entity entity, const ecs::Name& name, const auto&) {
					if (combo.item(name.name, entity == sceneViewEntity))
					{
						sceneViewEntity = entity;
						changed = true;
					}
				});
		}
	}

	ImGui::SetCursorPos(cursorPosition);

	return changed;
}

} // namespace spatial::ui