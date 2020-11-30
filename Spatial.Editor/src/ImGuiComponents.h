#pragma once

#include <spatial/common/Math.h>
#include <spatial/render/Name.h>
#include <spatial/render/Camera.h>
#include <spatial/render/Light.h>
#include <spatial/render/Stage.h>
#include <spatial/render/Transform.h>

namespace spatial::editor
{

void transformInput(Transform& transform, const std::string_view format);

void cameraInput(Camera& camera);

bool sceneHierarchy(Stage& registry, Actor& selectedActor);

bool inputText(const std::string_view label, std::string& value);

bool directionWidget(const std::string_view label, math::float3& v, float size = 100.0f,
					 std::uint32_t color = 0x22ff2200);

void lightInput(Light& light);

} // namespace spatial::editor