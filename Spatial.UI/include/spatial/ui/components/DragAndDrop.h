#pragma once

#include <filesystem>
#include <optional>
#include <string_view>
#include <fmt/format.h>

namespace spatial::ui
{

template <typename T>
struct DndId
{
	static constexpr std::string_view sName;
};

template <>
struct DndId<std::filesystem::path>
{
	static constexpr std::string_view sName = "DND_PATH";
};

template <>
struct DndId<std::string>
{
	static constexpr std::string_view sName = "DND_STRING";
};

class DragAndDropSource
{
  public:
	DragAndDropSource();
	~DragAndDropSource();

	[[nodiscard]] bool isStarted() const;

	void setPayload(std::string_view typeId, const void* data, size_t size) const;

	void setPayload(const std::filesystem::path& value) const
	{
		auto stringValue = value.string();
		setPayload(DndId<std::filesystem::path>::sName, stringValue.c_str(), stringValue.size());
	}

	void setPayload(const std::string& value) const
	{
		setPayload(DndId<std::string>::sName, value.c_str(), value.size());
	}

  private:
	bool mStarted;
};

class DragAndDropTarget
{
  public:
	DragAndDropTarget();
	~DragAndDropTarget();

	[[nodiscard]] bool isStarted() const;

	template <typename T>
	std::optional<T> getPayload() const
	{
		if (!mStarted)
			return std::nullopt;

		const auto& typeName = DndId<T>::sName;
		const auto* payload = ImGui::AcceptDragDropPayload(typeName.data());
		if (!payload)
			return std::nullopt;

		auto value = std::string{};
		value.reserve(static_cast<size_t>(payload->DataSize));

		const auto* data = reinterpret_cast<char*>(payload->Data);
		std::copy(data, data + payload->DataSize, std::back_inserter(value));

		return T{value};
	}

  private:
	bool mStarted;
};

} // namespace spatial::ui