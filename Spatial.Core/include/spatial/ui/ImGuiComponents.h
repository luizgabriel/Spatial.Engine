#pragma once

#include <entt/entity/registry.hpp>
#include <spatial/ecs/Components.h>

namespace spatial::ui
{

void transformInput(ecs::Transform& transform, const std::string_view format);

void cameraInput(ecs::Camera& camera);

bool sceneHierarchy(const entt::registry& registry, entt::entity& selectedEntity);

void inputText(const std::string_view label, std::string& value);

} // namespace spatial::ui