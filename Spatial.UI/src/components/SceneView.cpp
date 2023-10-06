#include <spatial/ui/components/Basic.h>
#include <spatial/ui/components/Combo.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <spatial/ecs/View.h>
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

    if (!registry.hasComponent<ecs::View>(sceneViewEntity)) {
        return;
    }

	const auto& sceneView = registry.getComponent<ecs::View>(sceneViewEntity);
	auto aspectRatio = 1.0;
	const auto* perspectiveCamera = registry.tryGetComponent<ecs::PerspectiveCamera>(sceneView.camera);
	if (perspectiveCamera != nullptr)
		aspectRatio = perspectiveCamera->aspectRatio;

	const auto imageSize = aspectRatio >= 1 ? math::uvec2{static_cast<double>(size.y) * aspectRatio, size.y}
											: math::uvec2{size.x, static_cast<double>(size.x) / aspectRatio};

	ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x + (size.x - static_cast<float>(imageSize.x)) * 0.5F,
									 ImGui::GetCursorScreenPos().y));

	auto colorAttachment = graphics::getTexture(registry, sceneView.colorAttachment);
	ui::image(colorAttachment, imageSize);
}

bool SceneView::selector(const ecs::Registry& registry, ecs::Entity& sceneViewEntity)
{
	auto windowWidth = ImGui::GetContentRegionAvail().x;
	auto sceneViewName = registry.tryGetComponent<ecs::Name>(sceneViewEntity);
	bool changed = false;

	auto cursorPosition = ImGui::GetCursorPos();
	constexpr auto comboWidth = 200;

	auto view = registry.getEntities<const ecs::Name, const ecs::View>();

	ImGui::SetCursorPos(ImVec2(windowWidth - comboWidth - 10, 30));

	if (view.size_hint() <= 1)
		return false;

	{
		ImGui::SetNextItemWidth(comboWidth);
		auto combo = ui::Combo{"##SceneViewSelector", sceneViewName ? sceneViewName->name : "No View Selected"};
		if (combo.isOpen())
		{
			registry.getEntities<const ecs::Name, const ecs::View>().each(
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