#pragma once

#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

template <typename... Components>
class UpdateController
{
  public:
	virtual ~UpdateController() = default;

	void onUpdateFrame(Registry& registry, float delta)
	{
		beforeUpdateEntities(registry);
		auto view = registry.template getEntities<Components...>();
		view.each([&](Entity entity, auto&... components) { onUpdateEntity(delta, entity, components...); });
	}

  protected:
	virtual void beforeUpdateEntities(Registry&){};
	virtual void onUpdateEntity(float delta, Components&... components) const noexcept = 0;
	virtual void onUpdateEntity(float delta, Entity, Components&... components) const noexcept
	{
		onUpdateEntity(delta, components...);
	}
};

} // namespace spatial::ecs