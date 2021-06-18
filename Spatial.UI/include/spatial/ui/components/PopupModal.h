#pragma once

#include <string_view>

namespace spatial::ui
{

class PopupModal
{
  public:
	PopupModal(const std::string_view name);
	~PopupModal();

	bool isOpen();

	void close();

  private:
	bool mOpened{false};
};

}