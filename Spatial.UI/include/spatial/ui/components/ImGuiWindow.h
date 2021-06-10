#pragma once

#include <imgui.h>
#include <string_view>
#include <spatial/common/Math.h>

namespace spatial::ui
{

class ImGuiWindow
{
  public:
	ImGuiWindow(const std::string_view name);
	~ImGuiWindow();

	math::float2 getSize();
};

} // namespace spatial::ui
