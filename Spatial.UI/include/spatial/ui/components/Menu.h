#pragma once

#include <string_view>

namespace spatial::ui
{

class Menu
{
  public:
	explicit Menu(std::string_view name);
	~Menu();

	[[nodiscard]] bool isOpen() const;

	[[nodiscard]] bool item(std::string_view name) const;

	[[nodiscard]] bool item(std::string_view name, std::string_view shortcut) const;

	static bool itemButton(std::string_view name);

	static bool itemButton(std::string_view name, std::string_view shortcut);

	template <typename Function>
	static void show(std::string_view name, Function func)
	{
		auto menu = ui::Menu{name};
		if (menu.isOpen())
			func();
	}

  private:
	bool mOpened{false};
};

} // namespace spatial::ui