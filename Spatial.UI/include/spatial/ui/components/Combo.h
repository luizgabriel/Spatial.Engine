#pragma once

#include <string_view>

namespace spatial::ui
{

class Combo
{
  public:
	Combo(std::string_view name, std::string_view selectedItem);
	~Combo();

	bool item(std::string_view name, bool isSelected);

	[[nodiscard]] bool isOpen() const;

  private:
	bool mIsOpen;
};

} // namespace spatial::ui