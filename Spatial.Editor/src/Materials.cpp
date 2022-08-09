#include "Materials.h"
#include <spatial/ecs/Resource.h>
#include <spatial/graphics/MathConversionUtils.h>
#include <spatial/graphics/Resources.h>
#include <spatial/graphics/TextureUtils.h>

namespace spatial::editor
{

const static auto gDefaultSampler =
	filament::TextureSampler{filament::TextureSampler::MagFilter::LINEAR, filament::TextureSampler::WrapMode::REPEAT};

void ColorMaterial::apply(filament::MaterialInstance& instance, const ecs::Registry&) const
{
	instance.setParameter("baseColor", toFilament(baseColor));
	instance.setParameter("metallic", metallic);
	instance.setParameter("roughness", roughness);
	instance.setParameter("reflectance", reflectance);
}

void GridMaterial::apply(filament::MaterialInstance& instance, const ecs::Registry&) const
{
	instance.setParameter("color", toFilament(color));
	instance.setParameter("scale", toFilament(scale));
	instance.setParameter("thickness", thickness);
}

void StandardOpaqueMaterial::apply(filament::MaterialInstance& instance, const ecs::Registry& registry) const
{
	const auto* dummyGray = graphics::getTexture(registry, "engine/gray");
	const auto* dummyWhite = graphics::getTexture(registry, "engine/white");
	const auto* dummyBlack = graphics::getTexture(registry, "engine/black");
	assert(dummyGray != nullptr);
	assert(dummyWhite != nullptr);
	assert(dummyBlack != nullptr);

	instance.setParameter("baseColor", toFilament(baseColor));
	const auto* albedoTexture = graphics::getTexture(registry, albedo);
	instance.setParameter("albedo", albedoTexture != nullptr ? albedoTexture : dummyGray, gDefaultSampler);
	instance.setParameter("tilingOffset", toFilament(math::vec4{tiling, offset}));

	instance.setParameter("metallic", metallic);
	const auto* metallicTexture = graphics::getTexture(registry, metallicMap);
	instance.setParameter("metallicMap", metallicTexture != nullptr ? metallicTexture : dummyWhite, gDefaultSampler);

	instance.setParameter("roughness", roughness);
	const auto* roughnessTexture = graphics::getTexture(registry, roughnessMap);
	instance.setParameter("roughnessMap", roughnessTexture != nullptr ? roughnessTexture : dummyWhite, gDefaultSampler);

	instance.setParameter("reflectance", reflectance);
	const auto* reflectanceTexture = graphics::getTexture(registry, reflectanceMap);
	instance.setParameter("reflectanceMap", reflectanceTexture != nullptr ? reflectanceTexture : dummyWhite,
						  gDefaultSampler);

	const auto* ambientOcclusionMapTexture = graphics::getTexture(registry, ambientOcclusionMap);
	instance.setParameter("ambientOcclusionMap",
						  ambientOcclusionMapTexture != nullptr ? ambientOcclusionMapTexture : dummyWhite,
						  gDefaultSampler);

	const auto* normalMapTexture = graphics::getTexture(registry, normalMap);
	instance.setParameter("normalMap", normalMapTexture != nullptr ? normalMapTexture : dummyWhite, gDefaultSampler);

	auto fixedEmissive = math::vec4{math::vec3{emissive}, 1.0f - emissive.w};
	instance.setParameter("emissive", toFilament(fixedEmissive));

	instance.setParameter("height", height);
	const auto* heightMapTexture = graphics::getTexture(registry, heightMap);
	instance.setParameter("heightMap", heightMapTexture != nullptr ? heightMapTexture : dummyBlack, gDefaultSampler);
}

void SkyBoxMaterial::apply(filament::MaterialInstance& instance, const ecs::Registry& registry) const
{
	instance.setParameter("showSun", showSun);
	instance.setParameter("color", toFilament(color));

	const auto* texture = graphics::getTexture(registry, skybox);
	instance.setParameter("constantColor", texture == nullptr);

	if (texture != nullptr)
	{
		instance.setParameter("skybox", texture, gDefaultSampler);
		return;
	}

	const auto* dummy = graphics::getTexture(registry, "engine/dummy_cubemap");
	assert(dummy != nullptr);
	instance.setParameter("skybox", dummy, gDefaultSampler);
}

} // namespace spatial::editor