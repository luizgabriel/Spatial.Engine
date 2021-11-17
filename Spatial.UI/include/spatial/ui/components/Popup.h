#pragma once

#include <string_view>

namespace spatial::ui
{

class Popup
{
  public:
	explicit Popup(std::string_view name, bool useContext = true);
	~Popup();

	[[nodiscard]] bool isOpen() const;

  private:
	bool mOpen;
};

}