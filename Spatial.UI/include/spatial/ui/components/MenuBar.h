#pragma once

#include <utility>
#include <functional>

namespace spatial::ui
{

class MenuBar
{
  public:
	MenuBar();
	~MenuBar();

	[[nodiscard]] bool isOpen() const
	{
		return mIsOpen;
	}

	template <typename Function>
	static void show(Function func)
	{
		auto menuBar = MenuBar{};
		if (menuBar.isOpen())
			std::invoke(std::forward<Function>(func));
	}

  private:
	bool mIsOpen;
};

} // namespace spatial::ui