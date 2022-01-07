#pragma once

#include <string_view>

namespace spatial::ui
{

class Collapse
{
  public:
	explicit Collapse(std::string_view name, bool defaultOpen = true);
	~Collapse();

	[[nodiscard]] bool isOpen() const;

	[[nodiscard]] bool onClose() const;

	template <typename Function>
	static void show(const std::string_view name, Function func)
	{
		auto collapse = Collapse{name};
		if (collapse.isOpen())
			func();
	}

  private:
	bool mOpened{true};
	bool mRendered{false};
};

} // namespace spatial::ui
