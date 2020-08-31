#include <filament/IndirectLight.h>
#include <math/mat3.h>
#include <spatial/ecs/Components.h>
#include <spatial/ecs/LightSystem.h>

namespace fl = filament;

namespace spatial::ecs
{

LightSystem::LightSystem(filament::Engine& engine) : mEngine{engine}, mLightManager{engine.getLightManager()}
{
}

void LightSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	auto& renderable = registry.get_or_emplace<ecs::SceneEntity>(entity);

	if (auto* light = registry.try_get<ecs::PointLight>(entity); light)
	{
		const auto& transform = registry.get_or_emplace<ecs::Transform>(entity);

		fl::LightManager::Builder(fl::LightManager::Type::POINT)
			.castShadows(light->castShadows)
			.intensity(light->intensity)
			.color(fl::Color::toLinear<fl::ACCURATE>(light->color))
			.position(transform.position)
			.build(mEngine, renderable.entity);
	}

	if (auto* light = registry.try_get<ecs::DirectionalLight>(entity); light)
	{
		registry.remove_if_exists<ecs::Transform>(entity);

		fl::LightManager::Builder(light->isSun ? fl::LightManager::Type::SUN : fl::LightManager::Type::DIRECTIONAL)
			.castShadows(light->castShadows)
			.intensity(light->intensity)
			.color(fl::Color::toLinear<fl::ACCURATE>(light->color))
			.direction(light->direction)
			.build(mEngine, renderable.entity);
	}

	if (auto* light = registry.try_get<ecs::SpotLight>(entity); light)
	{
		const auto& transform = registry.get_or_emplace<ecs::Transform>(entity);

		fl::LightManager::Builder(light->focused ? fl::LightManager::Type::FOCUSED_SPOT
												 : fl::LightManager::Type::DIRECTIONAL)
			.castShadows(light->castShadows)
			.intensity(light->intensity)
			.color(fl::Color::toLinear<fl::ACCURATE>(light->color))
			.direction(light->direction)
			.position(transform.position)
			.spotLightCone(light->innerCone, light->outerCone)
			.build(mEngine, renderable.entity);
	}
}

void LightSystem::onDestruct(entt::registry& registry, entt::entity entity)
{
	auto sceneEntity = registry.get<ecs::SceneEntity>(entity);
	mLightManager.destroy(sceneEntity.entity);
}

void LightSystem::onUpdate(entt::registry& registry)
{
	registry.view<ecs::PointLight, ecs::SceneEntity, ecs::Transform>().each(
		[this](const auto& light, const auto& sceneEntity, const auto& transform) {
			auto instance = mLightManager.getInstance(sceneEntity.entity);

			mLightManager.setPosition(instance, transform.position);
			mLightManager.setIntensity(instance, light.intensity);
			mLightManager.setColor(instance, fl::Color::toLinear<fl::ACCURATE>(light.color));
		});

	registry.view<ecs::DirectionalLight, ecs::SceneEntity>().each([this](const auto& light, const auto& sceneEntity) {
		auto instance = mLightManager.getInstance(sceneEntity.entity);

		mLightManager.setDirection(instance, light.direction);
		mLightManager.setIntensity(instance, light.intensity);
		mLightManager.setColor(instance, fl::Color::toLinear<fl::ACCURATE>(light.color));
	});

	registry.view<ecs::SpotLight, ecs::SceneEntity, ecs::Transform>().each([this](const auto& light, const auto& sceneEntity, const auto& transform) {
		auto instance = mLightManager.getInstance(sceneEntity.entity);

		mLightManager.setPosition(instance, transform.position);
		mLightManager.setDirection(instance, light.direction);
		mLightManager.setSpotLightCone(instance, light.innerCone, light.outerCone);
		mLightManager.setIntensity(instance, light.intensity);
		mLightManager.setColor(instance, fl::Color::toLinear<fl::ACCURATE>(light.color));
	});
}

} // namespace spatial::ecs
