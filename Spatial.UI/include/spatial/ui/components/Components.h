#pragma once

#include <filament/Texture.h>
#include <fmt/format.h>
#include <imgui.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/ecs/EntityName.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ecs/Transform.h>

namespace spatial::ui
{

bool inputText(std::string_view label, std::string& value);

void spacing(std::uint32_t times = 1);

template <typename Component>
void componentInput(ecs::Registry& registry, ecs::Entity entity);

template <typename... FilterComponents>
bool selectEntityInput(const std::string_view name, ecs::Registry& registry, ecs::Entity& selectedEntity)
{
	std::string selectedItemName;
	bool changed = false;

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
			if (ImGui::Selectable(entityName.name.data(), selected)) {
				selectedEntity = entity;
				changed = true;
			}

			if (selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	return changed;
}

void image(const filament::Texture& texture, math::float2 size = math::float2{0, 0}, math::float4 uv = math::float4{0, 0, 1, 1});

bool imageButton(const filament::Texture& texture, math::float2 size = math::float2{0, 0}, math::float4 uv = math::float4{0, 0, 1, 1});


} // namespace spatial::ui