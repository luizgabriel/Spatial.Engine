#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

class LightSystem
{
  public:
	explicit LightSystem(filament::Engine& engine);

	void synchronize(ecs::Registry& registry) const;

  private:
	filament::Engine& mEngine;

	void updateSunLights(ecs::Registry& registry) const;
	void createSunLights(ecs::Registry& registry) const;

	void updateDirectionalLights(ecs::Registry& registry) const;
	void createDirectionalLights(ecs::Registry& registry) const;

	void updatePointLights(ecs::Registry& registry) const;
	void createPointLights(ecs::Registry& registry) const;

	void createSpotLights(ecs::Registry& registry) const;
	void updateSpotLights(ecs::Registry& registry) const;
};

} // namespace spatial::render