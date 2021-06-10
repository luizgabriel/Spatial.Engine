#pragma once

#include <imgui.h>

namespace spatial::ui
{

class ImGuiDockSpace
{
  private:
	ImGuiDockNodeFlags mDockFlags;
	ImGuiWindowFlags mWindowFlags;

  public:
	ImGuiDockSpace();
	~ImGuiDockSpace();
};

} // namespace spatial::ui
