#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>

namespace spatial::render
{

class MaterialController
{
  public:
	explicit MaterialController(filament::Engine& engine) : mEngine{engine}
	{
	}

	template <typename MaterialComponent>
	void onUpdateFrame(ecs::Registry& registry, const filament::Material& material)
	{
		auto toBeAdded = registry.getEntities<MaterialComponent>(ecs::ExcludeComponents<render::MaterialInstance>);
		for (auto entity : toBeAdded)
			registry.addComponent<MaterialInstance>(entity, render::createMaterialInstance(mEngine, material));

		registry.getEntities<MaterialComponent, render::MaterialInstance>().each(
			[](const auto& data, auto& materialInstance) { data.setParameters(materialInstance.ref()); });

		// Clear Removed materials
		auto toBeRemoved = registry.getEntities<MaterialInstance>(ecs::ExcludeComponents<MaterialComponent>);
		registry.removeComponent<MaterialInstance>(toBeRemoved.begin(), toBeRemoved.end());
	}

  private:
	filament::Engine& mEngine;
};

} // namespace spatial::render