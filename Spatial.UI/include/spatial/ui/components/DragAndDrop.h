#pragma once

#include <string_view>
#include <filesystem>
#include <optional>

namespace spatial::ui
{

class DragAndDropSource
{
  public:
	DragAndDropSource();
	~DragAndDropSource();

	[[nodiscard]] bool isStarted() const;

	void setPayload(std::string_view name, const std::filesystem::path& path);

  private:
	bool mStarted;
};

class DragAndDropTarget
{
  public:
	DragAndDropTarget();
	~DragAndDropTarget();

	[[nodiscard]] bool isStarted() const;

	std::optional<std::filesystem::path> getPathPayload(std::string_view name) const;

  private:
	bool mStarted;
};


}