#pragma once

#include <spatial/ecs/Name.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ui/components/Combo.h>
#include <string>

namespace spatial::ui
{

class Search
{
  public:
	static bool searchText(std::string& search);

	template <typename... FilterComponents>
	static bool searchEntity(const std::string_view name, const ecs::Registry& registry, ecs::Entity& selectedEntity)
	{
		bool changed = false;

		const auto* selectedEntityName = registry.template tryGetComponent<const ecs::Name>(selectedEntity);

		{
			auto combo = Combo{name.data(), selectedEntityName ? selectedEntityName->c_str() : ""};
			if (combo.isOpen())
			{
				auto view = registry.getEntities<const ecs::Name, const FilterComponents...>();

				for (auto entity : view)
				{
					const auto& entityName = registry.getComponent<const ecs::Name>(entity);

					auto isSelected = selectedEntity == entity;
					if (combo.item(entityName.c_str(), isSelected))
					{
						selectedEntity = entity;
						changed = true;
					}
				}
			}
		}

		return changed;
	}
};

} // namespace spatial::ui