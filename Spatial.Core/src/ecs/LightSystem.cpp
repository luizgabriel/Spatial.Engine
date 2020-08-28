#include <spatial/ecs/LightSystem.h>
#include <spatial/ecs/Components.h>
#include <filament/IndirectLight.h>
#include <math/mat3.h>

namespace spatial::ecs
{

LightSystem::LightSystem(filament::Engine& engine)
	: mEngine{engine}, mLightManager{engine.getLightManager()}
{
}

void LightSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	const auto& light = registry.get<ecs::Light>(entity);
	const auto& transform = registry.get<ecs::Transform>(entity);

	auto renderable = createEntity(mEngine);
	registry.emplace<ecs::Renderable>(entity, std::move(renderable));
}

void LightSystem::onDestruct(entt::registry& registry, entt::entity entity)
{
	registry.remove<ecs::Renderable>(entity);
}

void LightSystem::onUpdate(entt::registry& registry)
{
	auto view = registry.view<ecs::Light, ecs::Transform, ecs::Renderable>();

	for (entt::entity entity : view) {
		const auto& light = view.get<ecs::Light>(entity);
		const auto& transform = view.get<ecs::Transform>(entity);
		auto& renderable = view.get<ecs::Renderable>(entity);

		auto instance = mLightManager.getInstance(renderable.entity.get());

		mLightManager.setPosition(instance, transform.position);
		mLightManager.setColor(instance, light.color);
		mLightManager.setIntensity(instance, light.intensity);
		mLightManager.setDirection(instance, light.direction);
		mLightManager.setSunAngularRadius(instance, light.sunAngularRadius);
		mLightManager.setSunHaloSize(instance, light.sunHaloSize);
		mLightManager.setSunHaloFalloff(instance, light.sunHaloFalloff);
		mLightManager.setShadowCaster(instance, renderable.castShadows);
		mLightManager.setSpotLightCone(instance, 3.14f /4.0f, 3.14f /4.0f);

		/*filament::LightManager::ShadowOptions options = mLightManager.getShadowOptions(instance);
		options.stable = light.stableShadowMap;
		options.normalBias = light.normalBias;
		options.constantBias = light.constantBias;
		options.polygonOffsetConstant = light.polygonOffsetConstant;
		options.polygonOffsetSlope = light.polygonOffsetSlope;
		options.screenSpaceContactShadows = renderable.screenSpaceContactShadows;
		options.stepCount = light.stepCount;
		options.maxShadowDistance = light.maxShadowDistance;
		mLightManager.setShadowOptions(instance, options);*/
	}
}

}
