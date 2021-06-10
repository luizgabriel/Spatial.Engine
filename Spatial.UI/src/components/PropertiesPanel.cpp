#include <spatial/ecs/EntityName.h>
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

	collapseComponentInput<ecs::Transform>("Transform", registry, selectedEntity);
	collapseComponentInput<ecs::PerspectiveCamera>("Perspective Camera", registry, selectedEntity);
	collapseComponentInput<ecs::OrthographicCamera>("Orthographic Camera", registry, selectedEntity);
	collapseComponentInput<ecs::CustomCamera>("Custom Camera", registry, selectedEntity);
	collapseComponentInput<ecs::DirectionalLight>("Directional Light", registry, selectedEntity);
	collapseComponentInput<ecs::SpotLight>("Spot Light", registry, selectedEntity);
	collapseComponentInput<ecs::PointLight>("Point Light", registry, selectedEntity);
	collapseComponentInput<ecs::Mesh>("Mesh", registry, selectedEntity);
}

} // namespace spatial::ui