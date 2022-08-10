#pragma once

#include <spatial/graphics/MaterialInstance.h>
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
		registry.getEntities<const MaterialComponent, const SharedMaterialInstance>().each(
			[&](const auto& data, const auto& materialInstance) {
				data.apply(materialInstance, std::as_const(registry));
			});
	}
};

} // namespace spatial::graphics