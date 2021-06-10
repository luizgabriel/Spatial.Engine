#pragma once

#include <string_view>

namespace spatial::ui
{

class ImGuiCollapse
{
  public:
	ImGuiCollapse(const std::string_view name, bool defaultOpen = true);
	~ImGuiCollapse();

	bool onClose() const;

  private:
	bool mOpened{true};
	bool mRendered{false};
};

} // namespace spatial::ui
