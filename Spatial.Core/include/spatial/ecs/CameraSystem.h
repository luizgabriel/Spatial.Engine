#pragma once

#include <entt/entity/registry.hpp>
#include <spatial/render/Camera.h>

namespace spatial::ecs
{

class CameraSystem
{
  public:
	CameraSystem(filament::Engine& engine);

	void onConstruct(entt::registry& registry, entt::entity entity);

	void onDestroy(entt::registry& registry, entt::entity entity);

	void onUpdate(const entt::registry& registry);

	const filament::Camera& get(entt::entity entity) const
	{
		return mCameraComponents.at(entity).ref();
	}

	filament::Camera& get(entt::entity entity)
	{
		return mCameraComponents.at(entity).ref();
	}

  private:
	filament::Engine& mEngine;
	std::unordered_map<entt::entity, spatial::Camera> mCameraComponents;
};

} // namespace spatial::ecs
