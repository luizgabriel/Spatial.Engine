#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace spatial::ui
{

class DragAndDropSource
{
  public:
	DragAndDropSource();
	~DragAndDropSource();

	[[nodiscard]] bool isStarted() const;

	void setPayload(std::string_view value) const;

  private:
	bool mStarted;
};

class DragAndDropTarget
{
  public:
	DragAndDropTarget();
	~DragAndDropTarget();

	[[nodiscard]] bool isStarted() const;

	[[nodiscard]] std::optional<std::string> getPayload() const;

  private:
	bool mStarted;
};

} // namespace spatial::ui