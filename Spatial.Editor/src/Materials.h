#pragma once

#include <filament/MaterialInstance.h>
#include <filament/TextureSampler.h>
#include <spatial/common/Math.h>
#include <spatial/resources/Resource.h>
#include <spatial/render/ResourceFinders.h>

namespace spatial::editor
{

struct ColorMaterial
{
	math::float3 baseColor{1.0f};
	float metallic{.8f};
	float roughness{0.8f};
	float reflectance{.1f};

	void apply(filament::MaterialInstance& instance) const;
};

struct GridMaterial
{
	math::float3 color{.5f, .5f, .5f};
	math::float2 scale{10.0f};
	float thickness{0.015f};

	void apply(filament::MaterialInstance& instance) const;
};

struct SkyBoxMaterial
{
	bool showSun{false};
	math::float4 color{.0f, .0f, .0f, 1.0f};
	Resource<ResourceType::CubeMapTexture> skybox{};

	template <typename Finder, typename = std::enable_if_t<render::is_cubemap_texture_finder_v<Finder>>>
	void apply(filament::MaterialInstance& instance, const Finder& finder) const
	{
		instance.setParameter("showSun", showSun);
		instance.setParameter("color", color);

		const auto* texture = finder(skybox);
		instance.setParameter("constantColor", texture == nullptr);

		const auto* dummy = finder(Resource<ResourceType::CubeMapTexture>{"engine://dummy_cubemap"});
		assert(dummy != nullptr);

		const auto sampler = filament::TextureSampler{filament::TextureSampler::MagFilter::LINEAR,
													  filament::TextureSampler::WrapMode::REPEAT};
		instance.setParameter("skybox", texture == nullptr ? dummy : texture, sampler);
	}
};

struct StandardOpaqueMaterial
{
	math::float3 baseColor{1.0f};
	Resource<ImageTexture> albedo{};

	math::float2 tiling{1.0f};
	math::float2 offset{.0f};

	float metallic{.0f};
	Resource<ImageTexture> metallicMap{};

	float roughness{1.0f};
	Resource<ImageTexture> roughnessMap{};

	float reflectance{.0f};
	Resource<ImageTexture> reflectanceMap{};

	bool useClearCoat{false};
	float clearCoat{.0f};
	float clearCoatRoughness{.0f};

	bool useAnisotropy{false};
	float anisotropy{.5f};
	Resource<ImageTexture> anisotropyDirectionMap{};

	Resource<ImageTexture> ambientOcclusionMap{};

	Resource<ImageTexture> normalMap{};

	Resource<ImageTexture> bentNormalMap{};

	math::float4 emissive{.0f};
	Resource<ImageTexture> emissiveMap;

	template <typename Finder, typename = std::enable_if_t<render::is_image_texture_finder_v<Finder>>>
	void apply(filament::MaterialInstance& instance, const Finder& finder) const
	{
		const auto* dummy = finder(Resource<ResourceType::CubeMapTexture>{"engine://dummy_texture"});
		assert(dummy != nullptr);

		const auto sampler = filament::TextureSampler{filament::TextureSampler::MagFilter::LINEAR,
													  filament::TextureSampler::WrapMode::REPEAT};

		instance.setParameter("baseColor", baseColor);
		const auto* albedoTexture = finder(albedo);
		instance.setParameter("albedo", albedoTexture != nullptr ? albedoTexture : dummy, sampler);
		instance.setParameter("tilingOffset", math::float4{tiling, offset});

		instance.setParameter("metallic", metallic);
		const auto* metallicTexture = finder(metallicMap);
		instance.setParameter("metallicMap", metallicTexture != nullptr ? metallicTexture : dummy, sampler);

		instance.setParameter("roughness", roughness);
		const auto* roughnessTexture = finder(roughnessMap);
		instance.setParameter("roughnessMap", roughnessTexture != nullptr ? roughnessTexture : dummy, sampler);

		instance.setParameter("reflectance", reflectance);
		const auto* reflectanceTexture = finder(reflectanceMap);
		instance.setParameter("reflectanceMap", reflectanceTexture != nullptr ? reflectanceTexture : dummy, sampler);

		instance.setParameter("useClearCoat", useClearCoat);
		instance.setParameter("clearCoat", clearCoat);
		instance.setParameter("clearCoatRoughness", clearCoatRoughness);

		instance.setParameter("useAnisotropy", useAnisotropy);
		instance.setParameter("anisotropy", anisotropy);
		const auto* anisotropyDirectionMapTexture = finder(anisotropyDirectionMap);
		instance.setParameter("anisotropyDirectionMap", anisotropyDirectionMapTexture != nullptr ? anisotropyDirectionMapTexture : dummy, sampler);

		const auto* ambientOcclusionMapTexture = finder(ambientOcclusionMap);
		instance.setParameter("ambientOcclusionMap", ambientOcclusionMapTexture != nullptr ? ambientOcclusionMapTexture : dummy, sampler);

		const auto* normalMapTexture = finder(normalMap);
		instance.setParameter("normalMap", normalMapTexture != nullptr ? normalMapTexture : dummy, sampler);

		const auto* bentNormalMapTexture = finder(bentNormalMap);
		instance.setParameter("bentNormalMap", bentNormalMapTexture != nullptr ? bentNormalMapTexture : dummy, sampler);

		instance.setParameter("emissive", emissive);
		const auto* emissiveMapTexture = finder(emissiveMap);
		instance.setParameter("emissiveMap", emissiveMapTexture != nullptr ? emissiveMapTexture : dummy, sampler);
	}
};

} // namespace spatial::editor