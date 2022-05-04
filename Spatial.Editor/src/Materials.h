#pragma once

#include <filament/MaterialInstance.h>
#include <filament/TextureSampler.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Registry.h>
#include <string_view>

namespace spatial::editor
{

struct ColorMaterial
{
	constexpr static auto typeName = "color_material";

	math::float3 baseColor{1.0f};
	float metallic{.8f};
	float roughness{0.8f};
	float reflectance{.1f};

	void apply(filament::MaterialInstance& instance, const ecs::Registry& registry) const;
};

struct GridMaterial
{
	constexpr static auto typeName = "grid_material";

	math::float3 color{.5f, .5f, .5f};
	math::float2 scale{10.0f};
	float thickness{0.015f};

	void apply(filament::MaterialInstance& instance, const ecs::Registry& registry) const;
};

struct SkyBoxMaterial
{
	constexpr static auto typeName = "skybox_material";

	bool showSun{false};
	math::float4 color{.0f, .0f, .0f, 1.0f};
	ecs::Entity skybox{ecs::NullEntity};

	void apply(filament::MaterialInstance& instance, const ecs::Registry& registry) const;
};

struct StandardOpaqueMaterial
{
	constexpr static auto typeName = "standard_opaque_material";

	math::float3 baseColor{1.0f};
	ecs::Entity albedo{ecs::NullEntity};

	math::float2 tiling{1.0f};
	math::float2 offset{.0f};

	float metallic{.0f};
	ecs::Entity metallicMap{ecs::NullEntity};

	float roughness{1.0f};
	ecs::Entity roughnessMap{ecs::NullEntity};

	float reflectance{.0f};
	ecs::Entity reflectanceMap{ecs::NullEntity};

	ecs::Entity ambientOcclusionMap{ecs::NullEntity};

	ecs::Entity normalMap{ecs::NullEntity};

	math::float4 emissive{.0f};

	float height{1.0f};
	ecs::Entity heightMap{ecs::NullEntity};

	void apply(filament::MaterialInstance& instance, const ecs::Registry& registry) const;
};

} // namespace spatial::editor