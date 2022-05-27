#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>

namespace spatial::graphics
{

struct CameraController
{
	static void createCameras(filament::Engine& engine, ecs::Registry& registry);
	static void updateCameras(ecs::Registry& registry);
	static void deleteCameras(ecs::Registry& registry);
};

} // namespace spatial::graphics