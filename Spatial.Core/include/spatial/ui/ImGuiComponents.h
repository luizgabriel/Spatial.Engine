#pragma once

#include <spatial/ecs/Components.h>

namespace spatial::ui
{

void transformInput(ecs::Transform& transform, const std::string_view format);

void cameraInput(ecs::Camera& camera);

}