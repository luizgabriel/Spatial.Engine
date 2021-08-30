#include "CustomComponents.h"
#include "DefaultMaterial.h"
#include "EditorCamera.h"
#include <imgui.h>

namespace spatial::ui
{

void defaultMaterialComponent(ecs::Registry& registry, ecs::Entity entity)
{
	auto& material = registry.getComponent<editor::DefaultMaterial>(entity);

	ImGui::ColorEdit3("Color", &material.baseColor.r);
	ImGui::DragFloat("Metallic", &material.metallic, 0.01f, .0f, 1.0f, "%.2f");
	ImGui::DragFloat("Reflectance", &material.reflectance, 0.01f, .0f, 1.0f, "%.2f");
	ImGui::DragFloat("Roughness", &material.roughness, 0.01f, .0f, 1.0f, "%.2f");
}

void editorCameraComponent(ecs::Registry& registry, ecs::Entity entity)
{
	auto& data = registry.getComponent<editor::EditorCamera>(entity);

	ImGui::DragFloat("Velocity", &data.velocity);
	ImGui::DragFloat("Sensitivity", &data.sensitivity);

	data.startPressed = ImGui::Button("Free Camera Control");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Left Ctrl + Left Mouse Click");
		ImGui::EndTooltip();
	}
}

} // namespace spatial::ui