#pragma once

#include <string_view>
#include <magic_enum.hpp>

namespace spatial::ui
{

class Combo
{
  public:
	Combo(std::string_view name, std::string_view selectedItem);
	~Combo();

	bool item(std::string_view name, bool isSelected);

	template <typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
	static bool fromEnum(const std::string_view name, E& selectedValue)
	{
		bool changed = false;
		if (auto combo = ui::Combo{name, magic_enum::enum_name(selectedValue).data()}; combo.isOpen())
		{
			for (auto [enumValue, enumName] : magic_enum::enum_entries<E>())
			{
				if (combo.item(enumName, enumValue == selectedValue))
				{
					selectedValue = enumValue;
					changed = true;
				}
			}
		}
		return changed;
	}

	[[nodiscard]] bool isOpen() const;

  private:
	bool mIsOpen;
};

} // namespace spatial::ui