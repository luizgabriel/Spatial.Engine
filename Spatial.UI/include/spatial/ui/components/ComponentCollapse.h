#pragma once

#include <spatial/ui/components/Collapse.h>

namespace spatial::ui
{

class ComponentCollapse
{
  public:
	ComponentCollapse(ecs::Registry& registry, ecs::Entity entity) : mRegistry{registry}, mEntity{entity}
	{
	}

	template <typename Component>
	bool hasComponentAndIsOpen(const std::string_view componentName)
	{
		auto isOpen = false;

		if (mRegistry.hasAllComponents<Component>(mEntity))
		{
			auto collapse = Collapse{componentName};
			isOpen = collapse.isOpen();

			if (collapse.onClose())
				mRegistry.removeComponent<Component>(mEntity);
		}

		return isOpen;
	}

  private:
	ecs::Registry& mRegistry;
	ecs::Entity mEntity;
};

} // namespace spatial::ui