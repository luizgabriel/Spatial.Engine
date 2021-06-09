#pragma once

#include <fmt/format.h>
#include <imgui.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/ecs/EntityName.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Transform.h>

namespace spatial::editor
{

bool inputText(const std::string_view label, std::string& value);

bool directionInput(const std::string_view label, math::float3& v, float size = 100.0f,
					 std::uint32_t color = 0x22ff2200);

bool buttonInput(const std::string_view label, float& value, float resetValue = .0f, float speed = .1f,
				 float min = .0f, float max = .0f, const std::string_view format = "%.2f");

bool vec2Input(const std::string_view label, math::float2& v, float resetValue = .0f, float speed = .1f,
			   float min = .0f, float max = .0f, const std::string_view format = "%.2f");

bool vec3Input(const std::string_view label, math::float3& v, float resetValue = .0f, float speed = .1f,
			   float min = .0f, float max = .0f, const std::string_view format = "%.2f");

bool vec4Input(const std::string_view label, math::float4& v, float resetValue = .0f, float speed = .1f,
			   float min = .0f, float max = .0f, const std::string_view format = "%.2f");

template <typename Component>
void componentInput(ecs::Registry& registry, ecs::Entity entity);

template <typename... FilterComponents>
void selectEntityInput(const std::string_view name, ecs::Registry& registry, ecs::Entity& selectedEntity)
{
	std::string selectedItemName = "";

	if (registry.isValid(selectedEntity) && registry.hasAllComponents<ecs::EntityName, FilterComponents...>(selectedEntity)) {
		const auto& entityName = registry.getComponent<const ecs::EntityName>(selectedEntity);
		selectedItemName = entityName;
	} else {
		selectedEntity = ecs::NullEntity;
	}

	if (ImGui::BeginCombo(name.data(), selectedItemName.data())) {

		auto view = registry.getEntities<ecs::EntityName, FilterComponents...>();

		for (auto entity : view) {

			auto& entityName = registry.getComponent<ecs::EntityName>(entity);

			auto selected = selectedEntity == entity;
			if (ImGui::Selectable(entityName.name.data(), selected))
				selectedEntity = entity;

			if (selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
}

template <typename Component>
void conditionalComponentInput(ecs::Registry& registry, ecs::Entity entity)
{
	if (registry.hasAllComponents<Component>(entity))
		componentInput<Component>(registry, entity);
}

template <typename Component>
void componentGroup(const std::string_view componentName, ecs::Registry& registry, ecs::Entity entity, bool defaultOpen = true)
{
	if (registry.hasAllComponents<Component>(entity))
	{
		bool openedHeader = true;
		if (ImGui::CollapsingHeader(componentName.data(), &openedHeader,
									defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None))
		{
			ImGui::Indent();

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::PushID(componentName.data());
			componentInput<Component>(registry, entity);
			ImGui::PopID();

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Unindent();
		}

		if (!openedHeader)
			registry.removeComponent<Component>(entity);
	}
}

void propertiesPanel(ecs::Registry& registry, ecs::Entity selectedEntity);

void sceneGraphPanel(const ecs::Registry& registry, ecs::Entity& selectedEntity);

void materialsPanel(ecs::Registry& registry);

} // namespace spatial::editor