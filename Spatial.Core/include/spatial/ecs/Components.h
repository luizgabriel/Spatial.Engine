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
	bool castShadows{true};
	bool receiveShadows{true};
	bool screenSpaceContactShadows{true};
};

struct Light
{
	filament::math::float3 direction{.0f};
	filament::math::float3 color{1.0f};
	float intensity{30000.0f};
	float sunAngularRadius{1.9f};
	float sunHaloSize{10.0f};
	float sunHaloFalloff{80.0f};

	bool stableShadowMap{false};
	float normalBias{1.0f};
	float constantBias{1.0f};
	float polygonOffsetConstant{.5f};
	float polygonOffsetSlope{2.0f};
	float maxShadowDistance{.3f};
	std::uint8_t stepCount{8};
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

	filament::math::float3 target{.0f};
	std::variant<Perspective, Ortographic, Custom> projection{Perspective{}};

	float* near() {
		return std::visit([](auto& proj) -> float* {
			return &proj.near;
		}, projection);
	}

	float* far() {
		return std::visit([](auto& proj) -> float* {
		  return &proj.far;
		}, projection);
	}

	float* aspectRatio() {
		return std::visit([](auto& proj) -> float* {
			if constexpr (std::is_same_v<decltype(proj), Perspective&> || std::is_same_v<decltype(proj), Ortographic&>){
				return &proj.aspectRatio;
			} else {
				return nullptr;
			}
		}, projection);
	}
};

} // namespace spatial::ecs