#include "Materials.h"
#include <spatial/ecs/Resource.h>
#include <spatial/render/Resources.h>
#include <spatial/render/TextureUtils.h>

namespace spatial::editor
{

const static auto gDefaultSampler = filament::TextureSampler{filament::TextureSampler::MagFilter::LINEAR,
													 filament::TextureSampler::WrapMode::REPEAT};

void ColorMaterial::apply(filament::MaterialInstance& instance, const ecs::Registry&) const
{
	instance.setParameter("baseColor", baseColor);
	instance.setParameter("metallic", metallic);
	instance.setParameter("roughness", roughness);
	instance.setParameter("reflectance", reflectance);
}

void GridMaterial::apply(filament::MaterialInstance& instance, const ecs::Registry&) const
{
	instance.setParameter("color", color);
	instance.setParameter("scale", scale);
	instance.setParameter("thickness", thickness);
}

void StandardOpaqueMaterial::apply(filament::MaterialInstance& instance, const ecs::Registry& registry) const
{
	const auto* dummyWhite = render::getTexture(registry, "engine/dummy_texture_white");
	const auto* dummyBlack = render::getTexture(registry, "engine/dummy_texture_black");
	assert(dummyWhite != nullptr);
	assert(dummyBlack != nullptr);

	instance.setParameter("baseColor", baseColor);
	const auto* albedoTexture = render::getTexture(registry, albedo);
	instance.setParameter("albedo", albedoTexture != nullptr ? albedoTexture : dummyWhite, gDefaultSampler);
	instance.setParameter("tilingOffset", math::float4{tiling, offset});

	instance.setParameter("metallic", metallic);
	const auto* metallicTexture = render::getTexture(registry, metallicMap);
	instance.setParameter("metallicMap", metallicTexture != nullptr ? metallicTexture : dummyWhite, gDefaultSampler);

	instance.setParameter("roughness", roughness);
	const auto* roughnessTexture = render::getTexture(registry, roughnessMap);
	instance.setParameter("roughnessMap", roughnessTexture != nullptr ? roughnessTexture : dummyWhite, gDefaultSampler);

	instance.setParameter("reflectance", reflectance);
	const auto* reflectanceTexture = render::getTexture(registry, reflectanceMap);
	instance.setParameter("reflectanceMap", reflectanceTexture != nullptr ? reflectanceTexture : dummyWhite,
						  gDefaultSampler);

	const auto* ambientOcclusionMapTexture = render::getTexture(registry, ambientOcclusionMap);
	instance.setParameter("ambientOcclusionMap",
						  ambientOcclusionMapTexture != nullptr ? ambientOcclusionMapTexture : dummyWhite,
						  gDefaultSampler);

	const auto* normalMapTexture = render::getTexture(registry,normalMap);
	instance.setParameter("normalMap", normalMapTexture != nullptr ? normalMapTexture : dummyWhite, gDefaultSampler);

	auto fixedEmissive = math::float4{emissive.xyz, 1.0f - emissive.w};
	instance.setParameter("emissive", fixedEmissive);

	instance.setParameter("height", height);
	const auto* heightMapTexture = render::getTexture(registry, heightMap);
	instance.setParameter("heightMap", heightMapTexture != nullptr ? heightMapTexture : dummyBlack, gDefaultSampler);
}

void SkyBoxMaterial::apply(filament::MaterialInstance& instance, const ecs::Registry& registry) const
{
	instance.setParameter("showSun", showSun);
	instance.setParameter("color", color);

	const auto* texture = render::getTexture(registry, skybox);
	instance.setParameter("constantColor", texture == nullptr);

	if (texture != nullptr)
	{
		instance.setParameter("skybox", texture, gDefaultSampler);
		return;
	}

	const auto* dummy = render::getTexture(registry, "engine/dummy_cubemap");
	assert(dummy != nullptr);
	instance.setParameter("skybox", dummy, gDefaultSampler);
}

} // namespace spatial::editor