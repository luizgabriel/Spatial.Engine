#pragma once

#include <spatial/ui/components/Window.h>
#include <string_view>

namespace spatial::ui
{

class ListPanel
{
  public:
	ListPanel(const std::string_view name);

	bool selectedNone();

	bool item(const std::string_view name, bool active);

  private:
	Window mWindow;
};

}