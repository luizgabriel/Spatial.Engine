#pragma once

#include <string_view>

namespace spatial::ui
{

class Menu
{
  public:
	Menu(const std::string_view name);
	~Menu();

	bool isOpen();

	bool item(const std::string_view name);

	bool item(const std::string_view name, const std::string_view shortcut);

  private:
	bool mOpened{false};
};

} // namespace spatial::ui