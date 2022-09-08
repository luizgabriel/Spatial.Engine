#pragma once

#include "Managers.h"
#include <glm/vec3.hpp>
#include <spatial/ecs/Registry.h>

namespace spatial::ui
{

struct EditorDragAndDrop
{
	static bool loadScene(std::string& scenePath);
	static void loadMeshInstance(ecs::Registry& registry, math::vec3 createEntityPosition = {});
	static void loadScriptResource(ecs::Registry& registry, ResourceManager::ResourceType& type);
};

} // namespace spatial::ui