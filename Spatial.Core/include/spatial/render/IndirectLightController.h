#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/resources/FileSystem.h>
#include <unordered_map>

namespace spatial::render
{

class IndirectLightController
{
  public:
	explicit IndirectLightController(filament::Engine& engine);

	void onUpdateFrame(ecs::Registry& registry);

  private:
	filament::Engine& mEngine;
};

} // namespace spatial::render