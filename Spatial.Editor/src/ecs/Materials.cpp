#include "Materials.h"
#include "Components.h"
#include <spatial/ecs/Resource.h>
#include <spatial/ecs/Texture.h>
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
	auto dummyWhite = graphics::getTexture<editor::tags::IsEditorWhiteTexture>(registry);
	auto dummyBlack = graphics::getTexture<editor::tags::IsEditorBlackTexture>(registry);
	assert(dummyWhite);
	assert(dummyBlack);

	instance->setParameter("baseColor", baseColor);
	auto albedoTexture = graphics::getTexture(registry, albedo);
	instance->setParameter("albedo", albedoTexture.value_or(*dummyWhite).get(), gDefaultSampler);
	instance->setParameter("tilingOffset", math::vec4{tiling, offset});

	instance->setParameter("metallic", metallic);
	auto metallicTexture = graphics::getTexture(registry, metallicMap);
	instance->setParameter("metallicMap", metallicTexture.value_or(*dummyWhite).get(), gDefaultSampler);

	instance->setParameter("roughness", roughness);
	auto roughnessTexture = graphics::getTexture(registry, roughnessMap);
	instance->setParameter("roughnessMap", roughnessTexture.value_or(*dummyWhite).get(), gDefaultSampler);

	instance->setParameter("reflectance", reflectance);
	auto reflectanceTexture = graphics::getTexture(registry, reflectanceMap);
	instance->setParameter("reflectanceMap", reflectanceTexture.value_or(*dummyWhite).get(), gDefaultSampler);

	auto ambientOcclusionMapTexture = graphics::getTexture(registry, ambientOcclusionMap);
	instance->setParameter("ambientOcclusionMap", ambientOcclusionMapTexture.value_or(*dummyWhite).get(),
						   gDefaultSampler);

	auto normalMapTexture = graphics::getTexture(registry, normalMap);
	instance->setParameter("normalMap", normalMapTexture.value_or(*dummyWhite).get(), gDefaultSampler);

	auto fixedEmissive = math::vec4{math::vec3{emissive}, 1.0f - emissive.w};
	instance->setParameter("emissive", fixedEmissive);

	instance->setParameter("height", height);
	auto heightMapTexture = graphics::getTexture(registry, heightMap);
	instance->setParameter("heightMap", heightMapTexture.value_or(*dummyBlack).get(), gDefaultSampler);
}

void SkyBoxMaterial::apply(const graphics::SharedMaterialInstance& instance, const ecs::Registry& registry) const
{
	auto dummy = graphics::getTexture<ecs::tags::IsDummyCubeMapTexture>(registry);
	assert(dummy);

	instance->setParameter("showSun", showSun);
	instance->setParameter("color", color);

	auto texture = graphics::getTexture(registry, skybox);
	instance->setParameter("constantColor", !texture.has_value());
	instance->setParameter("skybox", texture.value_or(std::move(*dummy)).get(), gDefaultSampler);
}

} // namespace spatial::editor