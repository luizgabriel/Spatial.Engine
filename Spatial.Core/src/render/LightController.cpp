#include <spatial/render/Entity.h>
#include <spatial/render/Light.h>
#include <spatial/render/LightController.h>
#include <spatial/render/Transform.h>

#include <spatial/ecs/Light.h>

namespace spatial::render
{

void update(const ecs::PointLight& data, Light& light)
{
	light.setFalloff(data.falloff);
	light.setIntensity(data.intensity);
	light.setColor(data.color);
}

void update(const ecs::DirectionalLight& data, Light& light)
{
	light.setIntensity(data.intensity);
	light.setDirection(data.direction);
	light.setColor(data.color);
	light.setShadowCaster(data.castShadows);
}

void update(const ecs::SunLight& data, Light& light)
{
	light.setSunHaloFalloff(data.haloFalloff);
	light.setSunHaloSize(data.haloSize);
	light.setSunAngularRadius(data.angularRadius);
	light.setIntensity(data.intensity);
	light.setColor(data.color);
}

void update(const ecs::SpotLight& data, Light& light)
{
	light.setFalloff(data.falloff);
	light.setDirection(data.direction);
	light.setIntensity(data.intensity);
	light.setColor(data.color);
	light.setSpotLightCone(data.innerAngle, data.outerAngle);
}

template <typename Component, Light::Type type>
void createLights(ecs::Registry& registry, filament::Engine& engine)
{
	auto view = registry.getEntities<Entity, Component>(ecs::ExcludeComponents<Light>);

	for (auto entity : view)
	{
		const auto& renderable = registry.getComponent<const Entity>(entity);
		registry.addComponent<Light>(entity, engine, renderable.get(), type);
	}
}

template <typename Component>
void updateLights(ecs::Registry& registry)
{
	auto view = registry.getEntities<Component, Light>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const Component>(entity);
		auto& light = registry.getComponent<Light>(entity);
		update(data, light);
	}
}

template <typename... Component>
void clearRemovedLights(ecs::Registry& registry)
{
	auto view = registry.getEntities<Light>(ecs::ExcludeComponents<Component...>);
	registry.removeComponent<Light>(view.begin(), view.end());
}

LightController::LightController(filament::Engine& engine) : mEngine{engine}
{
}

void LightController::onUpdateFrame(ecs::Registry& registry) const
{
	createLights<ecs::DirectionalLight, Light::Type::DIRECTIONAL>(registry, mEngine);
	createLights<ecs::PointLight, Light::Type::POINT>(registry, mEngine);
	createLights<ecs::SunLight, Light::Type::SUN>(registry, mEngine);
	createLights<ecs::SpotLight, Light::Type::SPOT>(registry, mEngine);

	updateLights<ecs::DirectionalLight>(registry);
	updateLights<ecs::PointLight>(registry);
	updateLights<ecs::SunLight>(registry);
	updateLights<ecs::SpotLight>(registry);

	clearRemovedLights<ecs::DirectionalLight, ecs::PointLight, ecs::SunLight, ecs::SpotLight>(registry);
}

} // namespace spatial::render