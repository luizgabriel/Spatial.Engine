#include <spatial/ecs/EntityName.h>
#include <spatial/ui/components/ComponentCollapse.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/Menu.h>
#include <spatial/ui/components/PropertiesPanel.h>

namespace spatial::ui
{

PropertiesPanel::PropertiesPanel(ecs::Registry& registry, ecs::Entity selectedEntity) : mWindow{"Properties"}
{
	bool opened = registry.isValid(selectedEntity);

	if (!opened)
	{
		ImGui::Text("No entity selected.");
		return;
	}

	if (registry.hasAllComponents<ecs::EntityName>(selectedEntity))
	{
		auto& node = registry.getComponent<ecs::EntityName>(selectedEntity);

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		inputText("##Name", node.name);
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::PopItemWidth();

	{
		auto collapse = ComponentCollapse{registry, selectedEntity};
		if (collapse.hasComponentAndIsOpen<ecs::Transform>("Transform"))
			componentInput<ecs::Transform>(registry, selectedEntity);

		if (collapse.hasComponentAndIsOpen<ecs::PerspectiveCamera>("Perspective Camera"))
			componentInput<ecs::PerspectiveCamera>(registry, selectedEntity);

		if (collapse.hasComponentAndIsOpen<ecs::OrthographicCamera>("Orthographic Camera"))
			componentInput<ecs::OrthographicCamera>(registry, selectedEntity);

		if (collapse.hasComponentAndIsOpen<ecs::CustomCamera>("Custom Camera"))
			componentInput<ecs::CustomCamera>(registry, selectedEntity);

		if (collapse.hasComponentAndIsOpen<ecs::DirectionalLight>("Directional Light"))
			componentInput<ecs::DirectionalLight>(registry, selectedEntity);

		if (collapse.hasComponentAndIsOpen<ecs::SpotLight>("Spot Light"))
			componentInput<ecs::SpotLight>(registry, selectedEntity);

		if (collapse.hasComponentAndIsOpen<ecs::PointLight>("Spot Light"))
			componentInput<ecs::PointLight>(registry, selectedEntity);

		if (collapse.hasComponentAndIsOpen<ecs::PointLight>("Point Light"))
			componentInput<ecs::PointLight>(registry, selectedEntity);

		if (collapse.hasComponentAndIsOpen<ecs::Mesh>("Mesh"))
			componentInput<ecs::Mesh>(registry, selectedEntity);
	}
}

} // namespace spatial::ui