#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>

namespace spatial::render
{

class MaterialInstancesSystem
{
  public:
	MaterialInstancesSystem(filament::Engine& engine) : mEngine{engine}
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

		registry.getEntities<const MaterialComponent, render::MaterialInstance>().each(
			[](const auto& data, auto& materialInstance) { data.setParameters(materialInstance.ref()); });
	}

  private:
	filament::Engine& mEngine;
};

} // namespace spatial::render