#include "Materials.h"
#include <spatial/ecs/Resource.h>
#include <spatial/graphics/Resources.h>
#include <spatial/graphics/TextureUtils.h>

namespace spatial::editor
{

const static auto gDefaultSampler =
	filament::TextureSampler{filament::TextureSampler::MagFilter::LINEAR, filament::TextureSampler::WrapMode::REPEAT};

void ColorMaterial::apply(const graphics::SharedMaterialInstance& instance, const ecs::Registry&) const
{
	instance->setParameter("baseColor", baseColor);
	instance->setParameter("metallic", metallic);
	instance->setParameter("roughness", roughness);
	instance->setParameter("reflectance", reflectance);
}

void GridMaterial::apply(const graphics::SharedMaterialInstance& instance, const ecs::Registry&) const
{
	instance->setParameter("color", color);
	instance->setParameter("scale", scale);
	instance->setParameter("thickness", thickness);
}

void StandardOpaqueMaterial::apply(const graphics::SharedMaterialInstance& instance,
								   const ecs::Registry& registry) const
{
	auto dummyGray = graphics::getTexture(registry, "engine/gray");
	auto dummyWhite = graphics::getTexture(registry, "engine/white");
	auto dummyBlack = graphics::getTexture(registry, "engine/black");
	assert(dummyGray.has_value());
	assert(dummyWhite.has_value());
	assert(dummyBlack.has_value());

	instance->setParameter("baseColor", baseColor);
	auto albedoTexture = graphics::getTexture(registry, albedo);
	instance->setParameter("albedo", albedoTexture ? *albedoTexture : *dummyGray, gDefaultSampler);
	instance->setParameter("tilingOffset", math::vec4{tiling, offset});

	instance->setParameter("metallic", metallic);
	auto metallicTexture = graphics::getTexture(registry, metallicMap);
	instance->setParameter("metallicMap", metallicTexture ? *metallicTexture : *dummyWhite, gDefaultSampler);

	instance->setParameter("roughness", roughness);
	auto roughnessTexture = graphics::getTexture(registry, roughnessMap);
	instance->setParameter("roughnessMap", roughnessTexture ? *roughnessTexture : *dummyWhite, gDefaultSampler);

	instance->setParameter("reflectance", reflectance);
	auto reflectanceTexture = graphics::getTexture(registry, reflectanceMap);
	instance->setParameter("reflectanceMap", reflectanceTexture ? *reflectanceTexture : *dummyWhite, gDefaultSampler);

	auto ambientOcclusionMapTexture = graphics::getTexture(registry, ambientOcclusionMap);
	instance->setParameter("ambientOcclusionMap",
						   ambientOcclusionMapTexture ? *ambientOcclusionMapTexture : *dummyWhite, gDefaultSampler);

	auto normalMapTexture = graphics::getTexture(registry, normalMap);
	instance->setParameter("normalMap", normalMapTexture ? *normalMapTexture : *dummyWhite, gDefaultSampler);

	auto fixedEmissive = math::vec4{math::vec3{emissive}, 1.0f - emissive.w};
	instance->setParameter("emissive", fixedEmissive);

	instance->setParameter("height", height);
	auto heightMapTexture = graphics::getTexture(registry, heightMap);
	instance->setParameter("heightMap", heightMapTexture ? *heightMapTexture : *dummyBlack, gDefaultSampler);
}

void SkyBoxMaterial::apply(const graphics::SharedMaterialInstance& instance, const ecs::Registry& registry) const
{
	instance->setParameter("showSun", showSun);
	instance->setParameter("color", color);

	auto texture = graphics::getTexture(registry, skybox);
	instance->setParameter("constantColor", !texture.has_value());

	if (texture)
	{
		instance->setParameter("skybox", *texture, gDefaultSampler);
		return;
	}

	auto dummy = graphics::getTexture(registry, "engine/dummy_cubemap");
	assert(dummy.has_value());
	instance->setParameter("skybox", *dummy, gDefaultSampler);
}

} // namespace spatial::editor