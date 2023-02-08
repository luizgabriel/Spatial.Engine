#include <imgui.h>
#include <spatial/ecs/Texture.h>
#include <spatial/ui/components/Basic.h>
#include <spatial/ui/components/CollapsingHeader.h>
#include <spatial/ui/components/DragAndDrop.h>

namespace spatial::ui
{

void spacing(std::uint32_t times)
{
	for (std::uint32_t i = 0; i < times; i++)
		ImGui::Spacing();
}

void separator(std::uint32_t spacing)
{
	ui::spacing(spacing);
	ImGui::Separator();
	ui::spacing(spacing);
}

int inputTextCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		auto* str = reinterpret_cast<std::string*>(data->UserData);
		str->resize(static_cast<unsigned long>(data->BufTextLen));
		data->Buf = str->data();
	}

	return 1;
};

bool inputText(const std::string_view label, std::string& value, std::string_view placeholder)
{
	return ImGui::InputTextWithHint(label.data(), placeholder.data(), value.data(), value.size() + 1,
									ImGuiInputTextFlags_CallbackResize, &inputTextCallback, &value);
}

bool inputPath(const std::string_view label, std::string& value, std::string_view placeholder)
{
	if (ui::inputText(label, value, placeholder))
		return true;

	{
		auto dnd = ui::DragAndDropTarget{};
		auto result = dnd.getPayload();
		if (result)
		{
			value = std::string{result->c_str()};
			return true;
		}
	}

	return false;
}

void image(graphics::OptionalTexture texture, math::vec2 size, math::vec4 uv)
{
	if (!texture)
		return;

	ImGui::Image(*texture, ImVec2(size.x, size.y), ImVec2(uv.x, uv.y), ImVec2(uv.z, uv.w));
}

bool imageButton(graphics::OptionalTexture texture, math::vec2 size, math::vec4 uv)
{
	if (!texture)
		return false;

	return ImGui::ImageButton(*texture, ImVec2(size.x, size.y), ImVec2(uv.x, uv.y), ImVec2(uv.z, uv.w));
}

void spanToAvailWidth(float weight)
{
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * weight);
}

} // namespace spatial::ui