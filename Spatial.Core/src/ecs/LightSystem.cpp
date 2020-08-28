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
	const auto& light = registry.get<ecs::Light>(entity);

	auto* renderable = registry.try_get<ecs::Renderable>(entity);

	utils::Entity lightEntity;
	if (renderable)
	{
		lightEntity = renderable->entity.get();
		mLightManager.destroy(lightEntity);
	}
	else
	{
		lightEntity = utils::EntityManager::get().create();
		renderable = &registry.emplace<ecs::Renderable>(entity, spatial::Entity{mEngine, lightEntity});
	}

	auto builder = std::visit(
		[&](const auto& type) -> fl::LightManager::Builder {
			using type_t = std::decay_t<decltype(type)>;
			if constexpr (std::is_same_v<type_t, ecs::Light::Point>)
			{
				const auto& transform = registry.get_or_emplace<ecs::Transform>(entity);

				return fl::LightManager::Builder(fl::LightManager::Type::POINT).position(transform.position);
			}
			else if constexpr (std::is_same_v<type_t, ecs::Light::Directional>)
			{
				registry.remove_if_exists<ecs::Transform>(entity);
				return fl::LightManager::Builder(type.isSun ? fl::LightManager::Type::SUN
															: fl::LightManager::Type::DIRECTIONAL)
					.direction(type.direction);
			}
			else if constexpr (std::is_same_v<type_t, ecs::Light::SpotLight>)
			{
				const auto& transform = registry.get_or_emplace<ecs::Transform>(entity);
				return fl::LightManager::Builder(type.focused ? fl::LightManager::Type::FOCUSED_SPOT
															  : fl::LightManager::Type::DIRECTIONAL)
					.direction(type.direction)
					.position(transform.position)
					.spotLightCone(type.innerCone, type.outerCone);
			}
		},
		light.type);

	builder.castShadows(renderable->castShadows)
		.intensity(light.intensity)
		.color(fl::Color::toLinear<fl::ACCURATE>(light.color))
		.build(mEngine, lightEntity);
}

void LightSystem::onDestruct(entt::registry& registry, entt::entity entity)
{
	registry.remove<ecs::Renderable>(entity);
}

void LightSystem::onUpdate(entt::registry& registry)
{
	auto view = registry.view<ecs::Light, ecs::Renderable>();

	for (entt::entity entity : view)
	{
		const auto& light = view.get<ecs::Light>(entity);
		auto& renderable = view.get<ecs::Renderable>(entity);
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
