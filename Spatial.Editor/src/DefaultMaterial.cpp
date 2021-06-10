#include "DefaultMaterial.h"

#include <imgui.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ui/components/ImGuiComponents.h>

namespace spatial::editor
{

void DefaultMaterial::setParameters(filament::MaterialInstance& instance) const
{
	instance.setParameter("baseColor", baseColor);
	instance.setParameter("metallic", metallic);
	instance.setParameter("roughness", roughness);
	instance.setParameter("reflectance", reflectance);
}

} // namespace spatial::editor

namespace spatial::ui
{

template <>
void componentInput<editor::DefaultMaterial>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& material = registry.getComponent<editor::DefaultMaterial>(entity);

	ImGui::ColorEdit3("Color", &material.baseColor.r);
	ImGui::DragFloat("Metallic", &material.metallic, 0.01f, .0f, 1.0f, "%.2f");
	ImGui::DragFloat("Reflectance", &material.reflectance, 0.01f, .0f, 1.0f, "%.2f");
	ImGui::DragFloat("Roughness", &material.roughness, 0.01f, .0f, 1.0f, "%.2f");
}

} // namespace spatial::ui