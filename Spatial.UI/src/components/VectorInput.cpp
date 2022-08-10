#include <imgui.h>
#include <imgui_internal.h>
#include <spatial/ui/components/VectorInput.h>

namespace spatial::ui
{

bool buttonInput(const std::string_view label, uint32_t& value, uint32_t resetValue, uint32_t speed, uint32_t min, uint32_t max)
{
	bool changed = false;
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.data());

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});
	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

	ImGui::PushFont(boldFont);
	if (ImGui::Button(label.data(), buttonSize))
	{
		changed = true;
		value = resetValue;
	}
	ImGui::PopFont();

	ImGui::SameLine();
	changed |= ImGui::DragInt("##ValueDrag", reinterpret_cast<int*>(&value), static_cast<float>(speed),
							  static_cast<int>(min), static_cast<int>(max));

	ImGui::PopStyleVar();

	ImGui::PopID();

	return changed;
}


bool buttonInput(const std::string_view label, float& value, float resetValue, float speed, float min, float max,
				 const std::string_view format)
{
	bool changed = false;
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.data());

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});
	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

	ImGui::PushFont(boldFont);
	if (ImGui::Button(label.data(), buttonSize))
	{
		changed = true;
		value = resetValue;
	}
	ImGui::PopFont();

	ImGui::SameLine();
	changed |= ImGui::DragFloat("##ValueDrag", &value, speed, min, max, format.data());

	ImGui::PopStyleVar();

	ImGui::PopID();

	return changed;
}


bool vec2Input(const std::string_view label, math::uvec2& v, uint32_t resetValue, uint32_t speed, uint32_t min, uint32_t max)
{
	bool changed = false;

	ImGui::PushID(label.data());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 70.0f);
	ImGui::AlignTextToFramePadding();
	ImGui::Text("%s", label.data());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	changed |= buttonInput("X", v.x, resetValue, speed, min, max);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	changed |= buttonInput("Y", v.y, resetValue, speed, min, max);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);

	ImGui::Columns(1);

	ImGui::PopID();

	return changed;
}

bool vec2Input(const std::string_view label, math::vec2& v, float resetValue, float speed, float min, float max,
			   const std::string_view format)
{
	bool changed = false;

	ImGui::PushID(label.data());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 70.0f);
	ImGui::AlignTextToFramePadding();
	ImGui::Text("%s", label.data());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	changed |= buttonInput("X", v.x, resetValue, speed, min, max, format);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	changed |= buttonInput("Y", v.y, resetValue, speed, min, max, format);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);

	ImGui::Columns(1);

	ImGui::PopID();

	return changed;
}

bool vec3Input(const std::string_view label, math::vec3& v, float resetValue, float speed, float min, float max,
			   const std::string_view format)
{
	bool changed = false;

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 70.0f);
	ImGui::Text("%s", label.data());
	ImGui::NextColumn();

	ImGui::PushID(label.data());
	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	changed |= buttonInput("X", v.x, resetValue, speed, min, max, format);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	changed |= buttonInput("Y", v.y, resetValue, speed, min, max, format);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.2f, 0.7f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.3f, 0.8f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.2f, 0.7f, 1.0f});
	changed |= buttonInput("Z", v.z, resetValue, speed, min, max, format);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);

	ImGui::Columns(1);

	ImGui::PopID();

	return changed;
}

bool vec4Input(const std::string_view label, math::vec4& v, float resetValue, float speed, float min, float max,
			   const std::string_view format)
{
	bool changed = false;

	ImGui::PushID(label.data());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 70.0f);
	ImGui::Text("%s", label.data());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	changed |= buttonInput("X", v.x, resetValue, speed, min, max, format);
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	changed |= buttonInput("Y", v.y, resetValue, speed, min, max, format);
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.2f, 0.7f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.3f, 0.8f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.2f, 0.7f, 1.0f});
	changed |= buttonInput("Z", v.z, resetValue, speed, min, max, format);
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.7f, 0.7f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.8f, 0.8f, 0.3f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.7f, 0.7f, 0.2f, 1.0f});
	changed |= buttonInput("W", v.w, resetValue, speed, min, max, format);
	ImGui::PopStyleColor(3);

	ImGui::Columns(1);

	ImGui::PopID();

	return changed;
}

} // namespace spatial::ui