#pragma once

#include <entt/entity/registry.hpp>
#include <spatial/ecs/Components.h>
#include <spatial/common/Math.h>

namespace spatial::ui
{

void transformInput(ecs::Transform& transform, const std::string_view format);

void cameraInput(ecs::Camera& camera);

bool sceneHierarchy(const entt::registry& registry, entt::entity& selectedEntity);

void inputText(const std::string_view label, std::string& value);

bool directionWidget(const std::string_view label, math::float3& v, float size = 100.0f, std::uint32_t color = 0xffff0000);

} // namespace spatial::ui