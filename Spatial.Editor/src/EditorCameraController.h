#pragma once

#include "ecs/Components.h"
#include <spatial/desktop/Window.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Transform.h>
#include <spatial/input/InputState.h>

namespace spatial::editor
{

struct EditorCameraController
{
	static void readCameraInputs(ecs::Registry& registry, const desktop::InputState& state);
	static void updateCameraTransforms(ecs::Registry& registry, float delta);
};

} // namespace spatial::editor