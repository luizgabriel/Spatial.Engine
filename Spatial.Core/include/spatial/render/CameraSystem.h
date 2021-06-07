#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

class CameraSystem
{
  public:
	explicit CameraSystem(filament::Engine& engine);

	void synchronize(ecs::Registry& registry) const;

  private:
	filament::Engine& mEngine;

	void createPerspectiveCameras(ecs::Registry& registry) const;
	void updatePerspectiveCameras(ecs::Registry& scene) const;

	void createOrthographicCameras(ecs::Registry& registry) const;
	void updateOrthographicCameras(ecs::Registry& scene) const;

	void createCustomCameras(ecs::Registry& registry) const;
	void updateCustomCameras(ecs::Registry& scene) const;
};

} // namespace spatial::render