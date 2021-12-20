#pragma once

#include <string_view>

namespace spatial::ui
{

class PopupModal
{
  public:
	explicit PopupModal(std::string_view name);
	~PopupModal();

	[[nodiscard]] bool isOpen() const;

	void close();

  private:
	bool mOpened{false};
};

} // namespace spatial::ui