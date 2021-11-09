#pragma once

#include "Collapse.h"
#include "ListPanel.h"
#include <fmt/format.h>
#include <imgui.h>
#include <filament/Texture.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/ecs/EntityName.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Transform.h>

namespace spatial::ui
{

bool inputText(const std::string_view label, std::string& value);

bool directionInput(const std::string_view label, math::float3& v, float size = 100.0f,
					std::uint32_t color = 0x22ff2200);

bool buttonInput(const std::string_view label, float& value, float resetValue = .0f, float speed = .1f, float min = .0f,
				 float max = .0f, const std::string_view format = "%.2f");

bool vec2Input(const std::string_view label, math::float2& v, float resetValue = .0f, float speed = .1f,
			   float min = .0f, float max = .0f, const std::string_view format = "%.2f");

bool vec3Input(const std::string_view label, math::float3& v, float resetValue = .0f, float speed = .1f,
			   float min = .0f, float max = .0f, const std::string_view format = "%.2f");

bool vec4Input(const std::string_view label, math::float4& v, float resetValue = .0f, float speed = .1f,
			   float min = .0f, float max = .0f, const std::string_view format = "%.2f");

void spacing(std::uint32_t times = 1);

template <typename Component>
void componentInput(ecs::Registry& registry, ecs::Entity entity);

template <typename... FilterComponents>
void selectEntityInput(const std::string_view name, ecs::Registry& registry, ecs::Entity& selectedEntity)
{
	std::string selectedItemName = "";

	if (registry.isValid(selectedEntity)
		&& registry.hasAllComponents<ecs::EntityName, FilterComponents...>(selectedEntity))
	{
		const auto& entityName = registry.getComponent<const ecs::EntityName>(selectedEntity);
		selectedItemName = entityName.name;
	}
	else
	{
		selectedEntity = ecs::NullEntity;
	}

	if (ImGui::BeginCombo(name.data(), selectedItemName.data()))
	{
		auto view = registry.getEntities<ecs::EntityName, FilterComponents...>();

		for (auto entity : view)
		{
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

template <typename... FilterComponents, typename... ExcludeComponents>
void entitiesListPanel(const std::string_view name, ecs::Registry& registry, ecs::Entity& selectedEntity, ecs::ExcludeComponentsType<ExcludeComponents...> exclude = {})
{
	auto panel = ListPanel{name};

	if (panel.selectedNone())
		selectedEntity = ecs::NullEntity;

	auto view = registry.getEntities<ecs::EntityName, FilterComponents...>(std::move(exclude));

	for (auto entity : view) {
		auto& component = registry.getComponent<ecs::EntityName>(entity);
		if (panel.item(component.name, selectedEntity == entity)) {
			selectedEntity = entity;
		}
	}
}

void image(const filament::Texture& texture, math::float2 size = math::float2{0, 0}, math::float4 uv = math::float4{0, 0, 1, 1});

bool imageButton(const filament::Texture& texture, math::float2 size = math::float2{0, 0}, math::float4 uv = math::float4{0, 0, 1, 1});

} // namespace spatial::ui