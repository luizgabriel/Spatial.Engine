#pragma once

#include <spatial/common/Math.h>
#include <string_view>

namespace spatial::ui
{

class Window
{
  public:
	explicit Window(const std::string_view name);
	~Window();

	[[nodiscard]] math::float2 getSize() const;
	void setSize(const math::float2& size);

	[[nodiscard]] math::float2 getPosition() const;
	void setPosition(const math::float2& size);

	[[nodiscard]] bool isOpen() const;

	[[nodiscard]] bool isFocused() const;

	template <typename Function>
	static void show(const std::string_view name, Function func)
	{
		auto window = Window{name};
		if (window.isOpen())
			std::invoke(std::forward<Function>(func));
	}

  private:
	bool mIsOpen;
};

} // namespace spatial::ui
