#pragma once

#include <string>
#include <variant>
#include <entt/entity/entity.hpp>
#include <math/vec3.h>
#include <math/mat4.h>
#include <spatial/render/Entity.h>

namespace spatial::ecs
{

struct Transform
{
	filament::math::float3 position{.0f};
	filament::math::float3 scale{1.0f};
	filament::math::float3 rotation{0.0f};
};

struct Renderable
{
	Entity entity;
};

struct Mesh
{
	std::string resourceFilePath;
};

struct Material
{
	entt::entity parentShape;
	std::string resourceFilePath;
	std::string name{"DefaultMaterial"};
};

struct Name
{
	std::string value;
};

struct Camera
{
	struct Perspective
	{
		float fieldOfView{45.f};
		float aspectRatio{16.0f / 9.0f};
		float near{0.1f};
		float far{1000000.0};
	};

	struct Ortographic
	{
		float aspectRatio{16.0f / 9.0f};
		float near{.0f};
		float far{1.0f};
	};

	struct Custom
	{
		filament::math::mat4 projection{};
		float near{.0f};
		float far{1.0f};
	};

	std::variant<Perspective, Ortographic, Custom> projection{Perspective{}};
	filament::math::float3 target{.0f};
};

} // namespace spatial::ecs