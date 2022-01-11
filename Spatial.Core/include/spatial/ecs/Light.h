#pragma once

#include <spatial/common/Math.h>
#include <spatial/common/StringHelpers.h>
#include <spatial/resources/Resource.h>

namespace spatial::ecs
{

struct PointLight
{
	float intensity{10000.0f};
	math::float3 color{1.0f, 1.0f, 1.0f};
	float falloff{1.0f};
};

struct SpotLight
{
	float intensity{10000.0f};
	math::float3 direction{.0f, -1.0f, .0f};
	math::float3 color{1.0f, 1.0f, 1.0f};
	bool castShadows{true};
	float falloff{1.0f};
	float innerAngle{math::pi / 4.0f};
	float outerAngle{math::pi / 2.0f};
};

struct DirectionalLight
{
	float intensity{10000.0f};
	math::float3 direction{.0f, -1.0f, .0f};
	math::float3 color{1.0f, 1.0f, 1.0f};
	bool castShadows{true};
};

struct SunLight
{
	float intensity{10000.0f};
	math::float3 color{1.0f, 1.0f, 1.0f};
	float angularRadius{0.545f};
	float haloFalloff{80.0f};
	float haloSize{10.0f};
	bool castShadows{true};
};

struct IndirectLight
{
	Resource<ResourceType::ImageTexture> reflectionsTexturePath{};
	Resource<ResourceType::ImageTexture> irradianceValuesPath{};
	float intensity{30000.0f};
};

} // namespace spatial::ecs