#include "Materials.h"

namespace spatial::editor
{

void ColorMaterial::apply(filament::MaterialInstance& instance) const
{
	instance.setParameter("baseColor", baseColor);
	instance.setParameter("metallic", metallic);
	instance.setParameter("roughness", roughness);
	instance.setParameter("reflectance", reflectance);
}

void GridMaterial::apply(filament::MaterialInstance& instance) const
{
	instance.setParameter("color", color);
	instance.setParameter("scale", scale);
	instance.setParameter("thickness", thickness);
}

void StandardOpaqueMaterial::apply(filament::MaterialInstance& instance, const render::ImageTextureFinder& finder) const
{
	const auto* dummyWhite = finder(ResourcePath<ResourceType::ImageTexture>{"engine/dummy_texture_white"});
	const auto* dummyBlack = finder(ResourcePath<ResourceType::ImageTexture>{"engine/dummy_texture_black"});
	assert(dummyWhite != nullptr);
	assert(dummyBlack != nullptr);

	const auto sampler = filament::TextureSampler{filament::TextureSampler::MagFilter::LINEAR,
												  filament::TextureSampler::WrapMode::REPEAT};

	instance.setParameter("baseColor", baseColor);
	const auto* albedoTexture = finder(albedo);
	instance.setParameter("albedo", albedoTexture != nullptr ? albedoTexture : dummyWhite, sampler);
	instance.setParameter("tilingOffset", math::float4{tiling, offset});

	instance.setParameter("metallic", metallic);
	const auto* metallicTexture = finder(metallicMap);
	instance.setParameter("metallicMap", metallicTexture != nullptr ? metallicTexture : dummyWhite, sampler);

	instance.setParameter("roughness", roughness);
	const auto* roughnessTexture = finder(roughnessMap);
	instance.setParameter("roughnessMap", roughnessTexture != nullptr ? roughnessTexture : dummyWhite, sampler);

	instance.setParameter("reflectance", reflectance);
	const auto* reflectanceTexture = finder(reflectanceMap);
	instance.setParameter("reflectanceMap", reflectanceTexture != nullptr ? reflectanceTexture : dummyWhite, sampler);

	const auto* ambientOcclusionMapTexture = finder(ambientOcclusionMap);
	instance.setParameter("ambientOcclusionMap",
						  ambientOcclusionMapTexture != nullptr ? ambientOcclusionMapTexture : dummyWhite, sampler);

	const auto* normalMapTexture = finder(normalMap);
	instance.setParameter("normalMap", normalMapTexture != nullptr ? normalMapTexture : dummyWhite, sampler);

	auto fixedEmissive = math::float4{emissive.xyz, 1.0f-emissive.w};
	instance.setParameter("emissive", fixedEmissive);

	instance.setParameter("height", height);
	const auto* heightMapTexture = finder(heightMap);
	instance.setParameter("heightMap", heightMapTexture != nullptr ? heightMapTexture : dummyBlack, sampler);
}

} // namespace spatial::editor