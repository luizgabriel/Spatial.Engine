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

	if (auto* light = registry.try_get<ecs::PointLight>(entity); light) {
		const auto& transform = registry.get_or_emplace<ecs::Transform>(entity);

		fl::LightManager::Builder(fl::LightManager::Type::POINT)
			.castShadows(renderable->castShadows)
			.intensity(light->intensity)
			.color(fl::Color::toLinear<fl::ACCURATE>(light->color))
			.position(transform.position)
			.build(mEngine, renderable.entity);
	}

	if (auto* light = registry.try_get<ecs::DirectionalLight>(entity); light) {
		registry.remove_if_exists<ecs::Transform>(entity);

		fl::LightManager::Builder(light->isSun ? fl::LightManager::Type::SUN
													: fl::LightManager::Type::DIRECTIONAL)
			.castShadows(renderable->castShadows)
			.intensity(light->intensity)
			.color(fl::Color::toLinear<fl::ACCURATE>(light->color))
			.direction(light->direction)
			.build(mEngine, renderable.entity);
	}

	if (auto* light = registry.try_get<ecs::SpotLight>(entity); light) {
		const auto& transform = registry.get_or_emplace<ecs::Transform>(entity);

		fl::LightManager::Builder(light->focused ? fl::LightManager::Type::FOCUSED_SPOT
													  : fl::LightManager::Type::DIRECTIONAL)
			.castShadows(renderable->castShadows)
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
	registry.remove<ecs::SceneEntity>(entity);
}

void LightSystem::onUpdate(entt::registry& registry)
{
	auto view = registry.view<ecs::Light, ecs::SceneEntity>();

	for (entt::entity entity : view)
	{
		const auto& light = view.get<ecs::Light>(entity);
		auto& renderable = view.get<ecs::SceneEntity>(entity);
		auto instance = mLightManager.getInstance(renderable.entity.get());

		mLightManager.setIntensity(instance, light.intensity);
		mLightManager.setColor(instance, fl::Color::toLinear<fl::ACCURATE>(light.color));

		std::visit([&](const auto& type){

		  using type_t = std::decay_t<decltype(type)>;

		  if constexpr (std::is_same_v<type_t, ecs::Light::Directional> || std::is_same_v<type_t, ecs::Light::SpotLight>)
			  mLightManager.setDirection(instance, type.direction);

		  if constexpr (std::is_same_v<type_t, ecs::Light::SpotLight>)
			  mLightManager.setSpotLightCone(instance, type.innerCone, type.outerCone);

		}, light.type);

		mLightManager.setShadowCaster(instance, renderable.castShadows);

		auto* transform = registry.try_get<ecs::Transform>(entity);
		if (transform) {
			mLightManager.setPosition(instance, transform->position);
		}
	}
}

} // namespace spatial::ecs
