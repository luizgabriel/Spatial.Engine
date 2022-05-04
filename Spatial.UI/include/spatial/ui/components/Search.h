#pragma once

#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ui/components/Combo.h>
#include <spatial/ui/components/DragAndDrop.h>
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

		const auto* selectedName = registry.tryGetComponent<const ecs::Name>(selectedEntity);
		const auto* selectedEntityName = selectedName ? selectedName->c_str() : "";

		{
			auto combo = Combo{name.data(), selectedEntityName};
			if (combo.isOpen())
			{
				auto view = registry.getEntities<const FilterComponents...>();

				for (auto entity : view)
				{
					const auto* nameComp = registry.tryGetComponent<const ecs::Name>(entity);
					const auto* entityName = nameComp ? nameComp->c_str() : "UNNAMED";

					auto isSelected = selectedEntity == entity;
					if (combo.item(entityName, isSelected))
					{
						selectedEntity = entity;
						changed = true;
					}
				}
			}
		}

		return changed;
	}

	template <typename... FilterComponents>
	static bool searchResource(const std::string_view name, ecs::Registry& registry, ecs::Entity& selectedEntity)
	{
		bool changed = searchEntity<FilterComponents...>(name, registry, selectedEntity);

		{
			auto dnd = ui::DragAndDropTarget{};
			auto result = dnd.getPayload();
			if (result)
			{
				selectedEntity = ecs::Resource::findOrCreate(registry, *result);
				changed = true;
			}
		}

		return changed;
	}
};

} // namespace spatial::ui