#pragma once

#include <filament/MaterialInstance.h>
#include <filament/TextureSampler.h>
#include <spatial/common/Math.h>
#include <string_view>

namespace spatial::editor
{

using ResourceFinder = std::function<const filament::Texture*(std::string_view)>;

struct ColorMaterial
{
	constexpr static auto typeName = "color_material";

	math::float3 baseColor{1.0f};
	float metallic{.8f};
	float roughness{0.8f};
	float reflectance{.1f};

	void apply(filament::MaterialInstance& instance) const;
};

struct GridMaterial
{
	constexpr static auto typeName = "grid_material";

	math::float3 color{.5f, .5f, .5f};
	math::float2 scale{10.0f};
	float thickness{0.015f};

	void apply(filament::MaterialInstance& instance) const;
};

struct SkyBoxMaterial
{
	constexpr static auto typeName = "skybox_material";

	bool showSun{false};
	math::float4 color{.0f, .0f, .0f, 1.0f};
	std::string skybox{};

	void apply(filament::MaterialInstance& instance, const ResourceFinder& finder) const;
};

struct StandardOpaqueMaterial
{
	constexpr static auto typeName = "standard_opaque_material";

	math::float3 baseColor{1.0f};
	std::string albedo{};

	math::float2 tiling{1.0f};
	math::float2 offset{.0f};

	float metallic{.0f};
	std::string metallicMap{};

	float roughness{1.0f};
	std::string roughnessMap{};

	float reflectance{.0f};
	std::string reflectanceMap{};

	std::string ambientOcclusionMap{};

	std::string normalMap{};

	math::float4 emissive{.0f};

	float height{1.0f};
	std::string heightMap{};

	void apply(filament::MaterialInstance& instance, const ResourceFinder& finder) const;
};

} // namespace spatial::editor