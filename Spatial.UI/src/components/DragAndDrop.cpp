#include <imgui.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <string>

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

void DragAndDropSource::setPayload(std::string_view typeId, const void * data, size_t size) const
{
	if (!isStarted())
		return;

	ImGui::SetDragDropPayload(typeId.data(), data, size);
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

} // namespace spatial::ui