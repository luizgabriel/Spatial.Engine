#pragma once

#include <spatial/ui/components/Combo.h>
#include <spatial/ecs/EntityName.h>
#include <spatial/ecs/Registry.h>
#include <string>

namespace spatial::ui
{

class Search
{
  public:
	static bool searchText(std::string& search);

	template <typename... FilterComponents>
	static bool searchEntity(const std::string_view name, ecs::Registry& registry, ecs::Entity& selectedEntity)
	{
		std::string selectedItemName;
		bool changed = false;

		if (registry.hasAllComponents<ecs::EntityName, FilterComponents...>(selectedEntity))
		{
			const auto& entityName = registry.getComponent<const ecs::EntityName>(selectedEntity);
			selectedItemName = entityName.name;
		}
		else
		{
			selectedEntity = ecs::NullEntity;
		}

		{
			auto combo = Combo{name.data(), selectedItemName.data()};
			if (combo.isOpen()) {
				auto view = registry.getEntities<ecs::EntityName, FilterComponents...>();

				for (auto entity : view)
				{
					auto& entityName = registry.getComponent<ecs::EntityName>(entity);

					auto selected = selectedEntity == entity;
					if (combo.item(entityName.c_str(), selected)) {
						selectedEntity = entity;
						changed = true;
					}
				}
			}
		}

		return changed;
	}
};

}