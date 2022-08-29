#pragma once

#include <string_view>

namespace spatial::ui
{

class TreeNode
{
  public:
	explicit TreeNode(std::string_view name, bool defaultOpen = true);
	~TreeNode();

	[[nodiscard]] bool isOpen() const;

	template <typename Function>
	static void show(const std::string_view name, Function func)
	{
		auto collapse = TreeNode{name};
		if (collapse.isOpen())
			func();
	}

  private:
	bool mOpened{true};
};

} // namespace spatial::ui
