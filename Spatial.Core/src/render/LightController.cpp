#include <spatial/render/Entity.h>
#include <spatial/render/Light.h>
#include <spatial/render/LightController.h>
#include <spatial/render/Transform.h>

#include <spatial/ecs/Light.h>
#include <spatial/render/Resources.h>
#include <spatial/render/SkyboxResources.h>

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
void createComponentLights(ecs::Registry& registry, filament::Engine& engine)
{
	auto view = registry.getEntities<Entity, Component>(ecs::ExcludeComponents<Light>);

	for (auto entity : view)
	{
		const auto& renderable = registry.getComponent<const Entity>(entity);
		registry.addComponent<Light>(entity, engine, renderable.get(), type);
	}
}

template <typename Component>
void updateComponentLights(ecs::Registry& registry)
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

void LightController::createLights(filament::Engine& engine, ecs::Registry& registry)
{
	registry.getEntities<const ecs::IndirectLight>(ecs::ExcludeComponents<IndirectLight>)
		.each([&](ecs::Entity entity, const ecs::IndirectLight& component) {
			const auto* reflectionsTexture =
				registry.tryGetComponent<const SharedTexture>(component.reflectionsTexture);

			if (!reflectionsTexture)
				return;

			auto builder = filament::IndirectLight::Builder()
							   .intensity(component.intensity)
							   .reflections(reflectionsTexture->get());

			const auto* irradianceValues = registry.tryGetComponent<bands_t>(component.irradianceValues);

			if (irradianceValues)
				builder = builder.irradiance(3, &irradianceValues->at(0));

			registry.addComponent<IndirectLight>(entity, engine, builder.build(engine));
		});

	createComponentLights<ecs::DirectionalLight, Light::Type::DIRECTIONAL>(registry, engine);
	createComponentLights<ecs::PointLight, Light::Type::POINT>(registry, engine);
	createComponentLights<ecs::SunLight, Light::Type::SUN>(registry, engine);
	createComponentLights<ecs::SpotLight, Light::Type::SPOT>(registry, engine);
}

void LightController::updateLights(ecs::Registry& registry)
{
	registry.getEntities<const ecs::IndirectLight, IndirectLight>().each(
		[&](const ecs::IndirectLight& component, IndirectLight& light) { light->setIntensity(component.intensity); });

	updateComponentLights<ecs::DirectionalLight>(registry);
	updateComponentLights<ecs::PointLight>(registry);
	updateComponentLights<ecs::SunLight>(registry);
	updateComponentLights<ecs::SpotLight>(registry);
}

void LightController::deleteLights(ecs::Registry& registry)
{
	auto view = registry.getEntities<IndirectLight>(ecs::ExcludeComponents<ecs::IndirectLight>);
	registry.removeComponent<Camera>(view.begin(), view.end());

	clearRemovedLights<ecs::DirectionalLight, ecs::PointLight, ecs::SunLight, ecs::SpotLight>(registry);
}

} // namespace spatial::render