#pragma once

#include "Icons.h"

#include <boost/algorithm/string/predicate.hpp>
#include <imgui.h>
#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Texture.h>
#include <spatial/graphics/TextureUtils.h>
#include <spatial/ui/components/Combo.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/PreviewTexture.h>
#include <string>

namespace spatial::ui
{

class Search
{
  public:
	static bool text(std::string& search, graphics::OptionalTexture icons);

	template <typename FilterComponent>
	static bool searchEntity(const std::string_view name, graphics::OptionalTexture icons,
							 const ecs::Registry& registry, ecs::Entity& selectedEntity)
	{
		bool changed = false;

		const auto* selectedName = registry.tryGetComponent<const ecs::Name>(selectedEntity);
		const auto* selectedEntityName = selectedName ? selectedName->c_str() : "";
		static std::string entitySearch;

		{
			auto combo = Combo{name.data(), selectedEntityName};
			if (combo.isOpen())
			{
				if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive()
					&& !ImGui::IsMouseClicked(ImGuiButtonFlags_MouseButtonLeft))
					ImGui::SetKeyboardFocusHere(1);

				text(entitySearch, icons);

				if (combo.item("##Empty", !registry.isValid(selectedEntity)))
				{
					selectedEntity = ecs::NullEntity;
					changed = true;
				}

				auto view = registry.getEntities<const FilterComponent>();

				for (auto entity : view)
				{
					const auto* nameComp = registry.tryGetComponent<const ecs::Name>(entity);

					if (nameComp && !boost::algorithm::icontains(nameComp->name.c_str(), entitySearch.c_str()))
						continue;

					const auto* entityName = nameComp ? nameComp->c_str() : "UNNAMED";

					if constexpr (std::is_same_v<FilterComponent, ecs::tags::IsImageTexture>)
					{
						previewTexture(registry, entity, icons, Icons::picture.uv());
						ImGui::SameLine(.0f, 5.0f);
					}

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

	template <typename ResourceTypeTag>
	static bool searchResource(const std::string_view name, graphics::OptionalTexture icons, ecs::Registry& registry,
							   ecs::Entity& selectedEntity)
	{
		bool changed = searchEntity<ResourceTypeTag>(name, std::move(icons), registry, selectedEntity);

		{
			auto dnd = ui::DragAndDropTarget{};
			auto result = dnd.getPayload();
			if (result)
			{
				selectedEntity = ecs::FileSystemResource::findOrCreate<ResourceTypeTag>(registry, *result);
				changed = true;
			}
		}

		return changed;
	}
};

} // namespace spatial::ui