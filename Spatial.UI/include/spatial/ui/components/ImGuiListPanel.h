#pragma once

#include <spatial/ui/components/ImGuiWindow.h>
#include <string_view>

namespace spatial::ui
{

class ImGuiListPanel
{
  public:
	ImGuiListPanel(const std::string_view name);

	bool selectedNone();

	bool addItem(const std::string_view name, bool active);

  private:
	ImGuiWindow mWindow;
};

}