#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>

namespace spatial::render
{

class MaterialController
{
  public:
	MaterialController(filament::Engine& engine) : mEngine{engine}
	{
	}

	template <typename MaterialComponent>
	void synchronize(ecs::Registry& registry, const filament::Material& material)
	{
		auto view = registry.getEntities<MaterialComponent>(ecs::ExcludeComponents<render::MaterialInstance>);
		for (auto entity : view)
		{
			registry.addComponent<MaterialInstance>(entity, render::createMaterialInstance(mEngine, material));
		}

		registry.getEntities<MaterialComponent, render::MaterialInstance>().each(
			[](const auto& data, auto& materialInstance) { data.setParameters(materialInstance.ref()); });
	}

	template <typename... MaterialComponents>
	void clearRemovedMaterials(ecs::Registry& registry)
	{
		auto view = registry.getEntities<MaterialInstance>(ecs::ExcludeComponents<MaterialComponents...>);
		registry.removeComponent<MaterialInstance>(view.begin(), view.end());
	}

  private:
	filament::Engine& mEngine;
};

} // namespace spatial::render