#pragma once

#include <spatial/ui/components/Window.h>
#include <spatial/ecs/Registry.h>

namespace spatial::ui
{

class PropertiesPanel
{
  public:
	PropertiesPanel(ecs::Registry& registry, ecs::Entity selectedEntity);

  private:
	Window mWindow;
};

} // namespace spatial::ui