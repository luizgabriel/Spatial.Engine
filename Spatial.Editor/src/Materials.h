#pragma once

#include <filament/MaterialInstance.h>
#include <filament/TextureSampler.h>
#include <spatial/common/Math.h>
#include <spatial/render/ResourceFinders.h>
#include <spatial/resources/ResourcePath.h>

namespace spatial::editor
{

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
	ResourcePath<ResourceType::CubeMapTexture> skybox{};

	template <typename Finder, typename = std::enable_if_t<render::is_cubemap_texture_finder_v<Finder>>>
	void apply(filament::MaterialInstance& instance, const Finder& finder) const
	{
		instance.setParameter("showSun", showSun);
		instance.setParameter("color", color);

		const auto* texture = finder(skybox);
		instance.setParameter("constantColor", texture == nullptr);

		const auto sampler = filament::TextureSampler{filament::TextureSampler::MagFilter::LINEAR,
													  filament::TextureSampler::WrapMode::REPEAT};
		if (texture != nullptr) {
			instance.setParameter("skybox", texture, sampler);
		} else {
			const auto* dummy = finder(ResourcePath<ResourceType::CubeMapTexture>{"engine/dummy_cubemap"});
			assert(dummy != nullptr);
			instance.setParameter("skybox", dummy, sampler);
		}
	}
};

struct StandardOpaqueMaterial
{
	constexpr static auto typeName = "standard_opaque_material";

	math::float3 baseColor{1.0f};
	ResourcePath<ImageTexture> albedo{};

	math::float2 tiling{1.0f};
	math::float2 offset{.0f};

	float metallic{.0f};
	ResourcePath<ImageTexture> metallicMap{};

	float roughness{1.0f};
	ResourcePath<ImageTexture> roughnessMap{};

	float reflectance{.0f};
	ResourcePath<ImageTexture> reflectanceMap{};

	ResourcePath<ImageTexture> ambientOcclusionMap{};

	ResourcePath<ImageTexture> normalMap{};

	math::float4 emissive{.0f};

	float height{1.0f};
	ResourcePath<ImageTexture> heightMap{};

	void apply(filament::MaterialInstance& instance, const render::ImageTextureFinder& finder) const;
};

} // namespace spatial::editor