#pragma once

#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

template <typename... Components>
class UpdateController
{
  public:
	virtual ~UpdateController() = default;

	template <typename... Args>
	void onUpdateFrame(Registry& registry, Args... args)
	{
		beforeUpdateEntities(registry);
		auto view = registry.template getEntities<Components...>();
		view.each([&](Entity entity, auto&... components) { onUpdateEntity(args..., entity, components...); });
	}

  protected:
	virtual void beforeUpdateEntities(Registry&)
	{
	}

	virtual void onUpdateEntity(float delta, Components&... components) const noexcept = 0;

	virtual void onUpdateEntity(float delta, Entity, Components&... components) const noexcept
	{
		onUpdateEntity(delta, components...);
	}
};

} // namespace spatial::ecs