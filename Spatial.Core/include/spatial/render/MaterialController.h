#pragma once

#include <filament/Engine.h>
#include <spatial/common/EventQueue.h>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>
#include <spatial/resources/FileSystem.h>
#include <unordered_map>

namespace spatial::render
{

struct LoadResourceEvent
{
	std::string texturePath;
};

struct MaterialController
{
	static void loadMaterials(filament::Engine& engine, FileSystem& fileSystem, ecs::Registry& registry);

	template <typename MaterialComponent>
	static void updateMaterial(ecs::Registry& registry)
	{
		registry.getEntities<MaterialComponent, SharedMaterialInstance>().each(
			[&](const auto& data, auto& materialInstance) { data.apply(*materialInstance->get(), std::as_const(registry)); });
	}
};

} // namespace spatial::render