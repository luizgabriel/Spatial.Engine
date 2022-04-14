#pragma once

#include <string_view>
#include <functional>

namespace spatial::ui
{

class Popup
{
  public:
	explicit Popup(std::string_view name, bool useContext = true);
	~Popup();

	[[nodiscard]] bool isOpen() const;

	template <typename Function>
	static void show(const std::string_view name, Function func)
	{
		auto popup = ui::Popup(name);
		if (popup.isOpen())
			std::invoke(std::forward<Function>(func));
	}

  private:
	bool mOpen;
};

} // namespace spatial::ui