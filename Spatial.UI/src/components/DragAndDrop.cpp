#include <spatial/ui/components/DragAndDrop.h>
#include <string>
#include <imgui.h>

namespace spatial::ui
{

DragAndDropSource::DragAndDropSource()
{
	mStarted = ImGui::BeginDragDropSource();
}

DragAndDropSource::~DragAndDropSource()
{
	if (mStarted)
		ImGui::EndDragDropSource();
}

void DragAndDropSource::setPayload(std::string_view name, const std::filesystem::path& path)
{
	const auto buffer = path.string();
	ImGui::SetDragDropPayload(name.data(), buffer.data(), buffer.size());
}

bool DragAndDropSource::isStarted() const
{
	return mStarted;
}

DragAndDropTarget::DragAndDropTarget()
{
	mStarted = ImGui::BeginDragDropTarget();
}

DragAndDropTarget::~DragAndDropTarget()
{
	if (mStarted)
		ImGui::EndDragDropTarget();
}

bool DragAndDropTarget::isStarted() const
{
	return mStarted;
}


std::optional<std::filesystem::path> DragAndDropTarget::getPathPayload(std::string_view name) const
{
	if (!mStarted) return std::nullopt;

	const auto* payload = ImGui::AcceptDragDropPayload(name.data());
	if (!payload) return std::nullopt;

	const auto* data = reinterpret_cast<char*>(payload->Data);
	auto value = std::string{};
	std::copy(data, data + payload->DataSize, std::back_inserter(value));

	return std::filesystem::path{std::move(value)};
}

}