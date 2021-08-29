#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Camera.h>
#include <spatial/render/Entity.h>

namespace spatial::render
{

class CameraController
{
  public:
	explicit CameraController(filament::Engine& engine);

	void synchronize(ecs::Registry& registry) const;

  private:
	filament::Engine& mEngine;

	void update(const ecs::PerspectiveCamera& data, Camera& camera) const;

	void update(const ecs::OrthographicCamera& data, Camera& camera) const;

	void update(const ecs::CustomCamera& data, Camera& camera) const;

	template <typename Component>
	void createCameras(ecs::Registry& registry) const
	{
		auto view = registry.getEntities<Entity, Component>(ecs::ExcludeComponents<Camera>);

		for (auto entity : view)
		{
			const auto& renderable = registry.getComponent<const Entity>(entity);
			registry.addComponent<Camera>(entity, mEngine, renderable.get());
		}
	}

	template <typename Component>
	void updateCameras(ecs::Registry& registry) const
	{
		auto view = registry.template getEntities<Component, Camera>();

		for (auto entity : view)
		{
			const auto& data = registry.getComponent<const Component>(entity);
			auto& camera = registry.getComponent<Camera>(entity);
			update(data, camera);
		}
	}

	template <typename... Components>
	void clearRemovedCameras(ecs::Registry& registry) const
	{
		auto view = registry.template getEntities<Camera>(ecs::ExcludeComponents<Components...>);
		registry.removeComponent<Camera>(view.begin(), view.end());
	}
};

} // namespace spatial::render