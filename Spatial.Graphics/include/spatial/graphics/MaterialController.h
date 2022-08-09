#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Registry.h>
#include <spatial/graphics/Resources.h>

namespace spatial::graphics
{

struct MaterialController
{
	static void loadMaterials(filament::Engine& engine, ecs::Registry& registry);

	static void createMaterialInstances(filament::Engine& engine, ecs::Registry& registry);

	template <typename MaterialComponent>
	static void updateMaterial(ecs::Registry& registry)
	{
		registry.getEntities<const MaterialComponent, SharedMaterialInstance>().each(
			[&](const auto& data, auto& materialInstance) {
				data.apply(*materialInstance->get(), std::as_const(registry));
			});
	}
};

} // namespace spatial::graphics