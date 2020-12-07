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

bool vec2Input(const std::string_view label, math::float2& v, float resetValue = .0f, const std::string_view format = "%.2f", float columnWidth = 70.0f);

bool vec3Input(const std::string_view label, math::float3& v, float resetValue = .0f, const std::string_view format = "%.2f", float columnWidth = 70.0f);

bool vec4Input(const std::string_view label, math::float4& v, float resetValue = .0f, const std::string_view format = "%.2f", float columnWidth = 70.0f);

} // namespace spatial::editor