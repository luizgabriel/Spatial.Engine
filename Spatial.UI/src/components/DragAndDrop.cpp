#include <imgui.h>
#include <iterator>
#include <spatial/ui/components/DragAndDrop.h>

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

bool DragAndDropSource::isStarted() const
{
	return mStarted;
}

void DragAndDropSource::setPayload(std::string_view data) const
{
	if (!isStarted())
		return;

	ImGui::SetDragDropPayload("DND_STRING", data.data(), data.size());
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

std::optional<std::string> DragAndDropTarget::getPayload() const
{
	if (!mStarted)
		return std::nullopt;

	const auto* payload = ImGui::AcceptDragDropPayload("DND_STRING");
	if (!payload)
		return std::nullopt;

	auto value = std::string{};
	value.reserve(static_cast<size_t>(payload->DataSize));

	const auto* data = reinterpret_cast<char*>(payload->Data);
	std::copy(data, data + payload->DataSize, std::back_inserter(value));

	return value;
}

} // namespace spatial::ui