#include <spatial/render/Entity.h>
#include <spatial/render/Light.h>
#include <spatial/render/LightSystem.h>
#include <spatial/render/Transform.h>

#include <spatial/ecs/Light.h>

namespace spatial::render
{

LightSystem::LightSystem(filament::Engine& engine) : mEngine{engine}
{
}

void LightSystem::synchronize(ecs::Registry& registry) const
{
	// TODO: All create functions are too similar. Can we merge then?
	createSunLights(registry);
	updateSunLights(registry);

	createDirectionalLights(registry);
	updateDirectionalLights(registry);

	createPointLights(registry);
	updatePointLights(registry);

	createSpotLights(registry);
	updateSpotLights(registry);
}

void LightSystem::updateSunLights(ecs::Registry& registry) const
{
    auto view = registry.getEntities<ecs::SunLight, Light>();

    for (auto entity : view)
    {
        const auto& data = registry.getComponent<const ecs::SunLight>(entity);
        auto& light = registry.getComponent<Light>(entity);

        light.setSunHaloFalloff(data.haloFalloff);
        light.setSunHaloSize(data.haloSize);
        light.setSunAngularRadius(data.angularRadius);
        light.setDirection(data.direction);
        light.setIntensity(data.intensity);
        light.setColor(data.color);
    }
}

void LightSystem::createSunLights(ecs::Registry& registry) const
{
    auto view = registry.getEntities<Entity, ecs::SunLight>(ecs::ExcludeComponents<Light>);

    for (auto entity : view)
    {
        const auto& renderable = registry.getComponent<const Entity>(entity);
        registry.addComponent<Light>(entity, mEngine, renderable.get());
    }
}

void LightSystem::createSpotLights(ecs::Registry& registry) const
{
	auto view = registry.getEntities<Entity, ecs::SpotLight>(ecs::ExcludeComponents<Light>);

	for (auto entity : view)
	{
		const auto& renderable = registry.getComponent<const Entity>(entity);
		registry.addComponent<Light>(entity, mEngine, renderable.get());
	}
}

void LightSystem::updateSpotLights(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::SpotLight, Light>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::SpotLight>(entity);
		auto& light = registry.getComponent<Light>(entity);

		light.setFalloff(data.falloff);
		light.setDirection(data.direction);
		light.setIntensity(data.intensity);
		light.setColor(data.color);
		light.setSpotLightCone(data.innerAngle, data.outerAngle);
	}
}

void LightSystem::createPointLights(ecs::Registry& registry) const
{
	auto view = registry.getEntities<Entity, ecs::PointLight>(ecs::ExcludeComponents<Light>);

	for (auto entity : view)
	{
		const auto& renderable = registry.getComponent<const Entity>(entity);
		registry.addComponent<Light>(entity, mEngine, renderable.get());
	}
}

void LightSystem::updatePointLights(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::PointLight, Light>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::PointLight>(entity);
		auto& light = registry.getComponent<Light>(entity);

		light.setFalloff(data.falloff);
		light.setIntensity(data.intensity);
		light.setColor(data.color);
		light.setShadowCaster(data.castShadows);
	}
}

void LightSystem::createDirectionalLights(ecs::Registry& registry) const
{
	auto view = registry.getEntities<Entity, ecs::DirectionalLight>(ecs::ExcludeComponents<Light>);

	for (auto entity : view)
	{
		const auto& renderable = registry.getComponent<const Entity>(entity);
		registry.addComponent<Light>(entity, mEngine, renderable.get());
	}
}

void LightSystem::updateDirectionalLights(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::DirectionalLight, Light>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::DirectionalLight>(entity);
		auto& light = registry.getComponent<Light>(entity);

		light.setIntensity(data.intensity);
		light.setDirection(data.direction);
		light.setColor(data.color);
		light.setShadowCaster(data.castShadows);
	}
}

} // namespace spatial::render