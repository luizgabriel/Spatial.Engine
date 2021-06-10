#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Light.h>

namespace spatial::render
{

class LightSystem
{
  public:
	explicit LightSystem(filament::Engine& engine);

	void synchronize(ecs::Registry& registry) const;

  private:
	filament::Engine& mEngine;

	void update(const ecs::PointLight& data, Light& light) const;

	void update(const ecs::DirectionalLight& data, Light& light) const;

	void update(const ecs::SunLight& data, Light& light) const;

	void update(const ecs::SpotLight& data, Light& light) const;

	template <typename Component, Light::Type type>
	void createLights(ecs::Registry& registry) const
	{
		auto view = registry.getEntities<Entity, Component>(ecs::ExcludeComponents<Light>);

		for (auto entity : view)
		{
			const auto& renderable = registry.getComponent<const Entity>(entity);
			registry.addComponent<Light>(entity, mEngine, renderable.get(), type);
		}
	}

	template <typename Component>
	void updateLights(ecs::Registry& registry) const
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
	void clearRemovedLights(ecs::Registry& registry) const
	{
		auto view = registry.getEntities<Light>(ecs::ExcludeComponents<Component...>);
		registry.removeComponent<Light>(view.begin(), view.end());
	}
};

} // namespace spatial::render