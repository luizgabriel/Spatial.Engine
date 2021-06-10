#pragma once

#include "ImGuiWindow.h"
#include <spatial/ecs/Registry.h>

namespace spatial::ui
{

class ImGuiPropertiesPanel
{
  public:
	ImGuiPropertiesPanel(ecs::Registry& registry, ecs::Entity selectedEntity);

  private:
	ImGuiWindow mWindow;
};

} // namespace spatial::ui