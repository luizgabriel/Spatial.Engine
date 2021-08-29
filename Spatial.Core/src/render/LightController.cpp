#include <spatial/render/Entity.h>
#include <spatial/render/Light.h>
#include <spatial/render/LightController.h>
#include <spatial/render/Transform.h>

#include <spatial/ecs/Light.h>

namespace spatial::render
{

LightController::LightController(filament::Engine& engine) : mEngine{engine}
{
}

void LightController::update(const ecs::PointLight& data, Light& light) const
{
	light.setFalloff(data.falloff);
	light.setIntensity(data.intensity);
	light.setColor(data.color);
}

void LightController::update(const ecs::DirectionalLight& data, Light& light) const
{
	light.setIntensity(data.intensity);
	light.setDirection(data.direction);
	light.setColor(data.color);
	light.setShadowCaster(data.castShadows);
}

void LightController::update(const ecs::SunLight& data, Light& light) const
{
	light.setSunHaloFalloff(data.haloFalloff);
	light.setSunHaloSize(data.haloSize);
	light.setSunAngularRadius(data.angularRadius);
	light.setIntensity(data.intensity);
	light.setColor(data.color);
}

void LightController::update(const ecs::SpotLight& data, Light& light) const
{
	light.setFalloff(data.falloff);
	light.setDirection(data.direction);
	light.setIntensity(data.intensity);
	light.setColor(data.color);
	light.setSpotLightCone(data.innerAngle, data.outerAngle);
}

void LightController::synchronize(ecs::Registry& registry) const
{
	createLights<ecs::DirectionalLight, Light::Type::DIRECTIONAL>(registry);
	updateLights<ecs::DirectionalLight>(registry);

	createLights<ecs::PointLight, Light::Type::POINT>(registry);
	updateLights<ecs::PointLight>(registry);

	createLights<ecs::SunLight, Light::Type::SUN>(registry);
	updateLights<ecs::SunLight>(registry);

	createLights<ecs::SpotLight, Light::Type::SPOT>(registry);
	updateLights<ecs::SpotLight>(registry);

	clearRemovedLights<ecs::DirectionalLight, ecs::PointLight, ecs::SunLight, ecs::SpotLight>(registry);
}

} // namespace spatial::render