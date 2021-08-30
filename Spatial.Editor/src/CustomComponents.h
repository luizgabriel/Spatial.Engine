#pragma once

#include <spatial/ecs/Registry.h>
namespace spatial::ui
{

void defaultMaterialComponent(ecs::Registry& registry, ecs::Entity entity);

void editorCameraComponent(ecs::Registry& registry, ecs::Entity entity);

}