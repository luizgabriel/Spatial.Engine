#pragma once

#include <string_view>

namespace spatial::ui
{

class DockSpace
{
  public:
	DockSpace(const std::string_view name);
	~DockSpace();
};

} // namespace spatial::ui
