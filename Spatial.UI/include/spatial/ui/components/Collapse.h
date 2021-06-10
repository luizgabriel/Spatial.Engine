#pragma once

#include <string_view>

namespace spatial::ui
{

class Collapse
{
  public:
	Collapse(const std::string_view name, bool defaultOpen = true);
	~Collapse();

	bool isOpen() const;

	bool onClose() const;

  private:
	bool mOpened{true};
	bool mRendered{false};
};

} // namespace spatial::ui
