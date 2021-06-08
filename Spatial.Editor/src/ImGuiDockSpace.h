#pragma once

#include <imgui.h>

namespace spatial::editor
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

} // namespace spatial::editor
