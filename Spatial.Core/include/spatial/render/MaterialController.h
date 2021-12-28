#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>
#include <spatial/ecs/Tags.h>

namespace spatial::render
{

class MaterialController
{
  public:
	explicit MaterialController(filament::Engine& engine) : mEngine{engine}
	{
	}

	template <typename MaterialComponent, typename... Args>
	void onUpdateFrame(ecs::Registry& registry, const filament::Material& material, Args&&... args)
	{
		registry.getEntities<const MaterialComponent>(ecs::ExcludeComponents<render::MaterialInstance>)
			.each([&](ecs::Entity entity, const auto& component) {
				auto materialInstance = render::createMaterialInstance(mEngine, material);
				component.apply(materialInstance.ref(), std::forward<Args>(args)...);
				registry.addComponent<MaterialInstance>(entity, std::move(materialInstance));
			});

		registry.getEntities<MaterialComponent, render::MaterialInstance>().each(
			[&](const auto& data, auto& materialInstance) {
				data.apply(materialInstance.ref(), std::forward<Args>(args)...);
			});
	}

  private:
	filament::Engine& mEngine;
};

} // namespace spatial::render