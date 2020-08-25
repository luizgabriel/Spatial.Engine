#include <imgui.h>
#include <spatial/ui/ImGuiComponents.h>
#include <array>

namespace spatial::ui
{

void transformInput(ecs::Transform& transform, const std::string_view format)
{
	for (char c : format)
	{
		switch (c)
		{
		case 'p':
			ImGui::DragFloat3("Position", &transform.position[0], .001f);
			break;
		case 'r':
			ImGui::DragFloat3("Rotation", &transform.rotation[0], .001f);
			break;
		case 's':
			ImGui::DragFloat3("Scale", &transform.scale[0], .001f);
			break;
		}
	}
}

void cameraInput(ecs::Camera& camera)
{
	auto isPerspective = std::holds_alternative<ecs::Camera::Perspective>(camera.projection);
	auto isOrthographic = std::holds_alternative<ecs::Camera::Ortographic>(camera.projection);
	auto isCustom = std::holds_alternative<ecs::Camera::Custom>(camera.projection);
	auto options = std::array{"Perspective", "Orthographic", "Custom"};
	int currentItem = -1;

	if (isPerspective) currentItem = 0;
	else if (isOrthographic) currentItem = 1;
	else if (isCustom) currentItem = 2;

	if (ImGui::Combo("Projection", &currentItem, &options[0], options.size())) {
		if (currentItem == 0)
			camera.projection = ecs::Camera::Perspective{};
		else if (currentItem == 1)
			camera.projection = ecs::Camera::Ortographic{};
		else if (currentItem == 2)
			camera.projection = ecs::Camera::Custom{};
	}

	if (currentItem == 0) {
		auto& proj = std::get<ecs::Camera::Perspective>(camera.projection);
		ImGui::SliderFloat("Field of View", &proj.fieldOfView, 15.0f, 120.0f);
		ImGui::InputFloat("Aspect Ratio", &proj.aspectRatio);
		ImGui::InputFloat("Near", &proj.near);
		ImGui::InputFloat("Far", &proj.far);
	} else if (currentItem == 1) {
		auto& proj = std::get<ecs::Camera::Ortographic>(camera.projection);
		ImGui::InputFloat("Aspect Ratio", &proj.aspectRatio);
		ImGui::InputFloat("Near", &proj.near);
		ImGui::InputFloat("Far", &proj.far);
	} else if (currentItem == 2) {
		auto& proj = std::get<ecs::Camera::Custom>(camera.projection);
		ImGui::InputFloat("Near", &proj.near);
		ImGui::InputFloat("Far", &proj.far);
	}
}

} // namespace spatial::ui