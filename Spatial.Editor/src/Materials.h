#pragma once

#include <filament/MaterialInstance.h>
#include <filament/TextureSampler.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Resource.h>

namespace spatial::editor
{

struct DefaultMaterial
{
	math::float3 baseColor{1.0f};
	float metallic{.8f};
	float roughness{0.8f};
	float reflectance{.1f};

	void apply(filament::MaterialInstance& instance) const;
};

struct SkyBoxMaterial
{
	bool showSun{false};
	math::float4 color{.0f, .0f, .0f, 1.0f};
	ecs::Resource<ecs::ResourceType::CubeMapTexture> skybox{};

	template <typename Finder>
	void apply(filament::MaterialInstance& instance, const Finder& finder) const
	{
		instance.setParameter("showSun", showSun);
		instance.setParameter("color", color);

		const auto* texture = finder(skybox);
		instance.setParameter("constantColor", texture == nullptr);

		const auto* dummy = finder(ecs::Resource<ecs::ResourceType::CubeMapTexture>{"engine://dummy_cubemap"});
		assert(dummy != nullptr);

		const auto sampler = filament::TextureSampler{filament::TextureSampler::MagFilter::LINEAR,
													  filament::TextureSampler::WrapMode::REPEAT};
		instance.setParameter("skybox", texture == nullptr ? dummy : texture, sampler);
	}
};

} // namespace spatial::editor