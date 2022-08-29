#pragma once

#include <string_view>

namespace spatial::ui
{

class CollapsingHeader
{
  public:
	explicit CollapsingHeader(std::string_view name, bool defaultOpen = true);
	~CollapsingHeader();

	[[nodiscard]] bool isOpen() const;

	[[nodiscard]] bool onClose() const;

	template <typename Function>
	static void show(const std::string_view name, Function func)
	{
		auto collapse = CollapsingHeader{name};
		if (collapse.isOpen())
			func();
	}

  private:
	bool mOpened{true};
	bool mRendered{false};
};

} // namespace spatial::ui
