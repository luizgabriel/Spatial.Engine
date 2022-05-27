#pragma once

#include <spatial/ui/components/styles/WindowPaddingStyle.h>
#include <string_view>

namespace spatial::ui
{

class PopupModal
{
  public:
	explicit PopupModal(std::string_view name);
	~PopupModal();

	template <typename Func>
	static void show(std::string_view name, Func func)
	{
		auto style = ui::WindowPaddingStyle{math::vec2{10.0f}};
		auto modal = PopupModal{name};
		if (modal.isOpen())
			func();
	}

	[[nodiscard]] bool isOpen() const;

	void close() const;

	static void open(std::string_view name);

  private:
	bool mOpened{false};
};

} // namespace spatial::ui