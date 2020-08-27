#include <array>
#include <imgui.h>
#include <spatial/ui/ImGuiComponents.h>

namespace spatial::ui
{

template <typename ValueType>
constexpr ValueType pi = static_cast<ValueType>(3.14159265359L);

void transformInput(ecs::Transform& transform, const std::string_view format)
{
	for (char c : format)
	{
		switch (c)
		{
		case 'p':
			ImGui::DragFloat3("Position", &transform.position[0]);
			break;
		case 'r':
			ImGui::DragFloat3("Rotation", &transform.rotation[0], pi<double> / 20.0, .0f, .0f);
			break;
		case 'y':
			ImGui::DragFloat2("Rotation", &transform.rotation[0], pi<double> / 20.0, .0f, .0f);
			break;
		case 's':
			ImGui::DragFloat3("Scale", &transform.scale[0]);
			break;
		}
	}
}

void cameraInput(ecs::Camera& camera)
{
	ImGui::InputFloat3("Target", &camera.target[0]);

	auto isPerspective = std::holds_alternative<ecs::Camera::Perspective>(camera.projection);
	auto isOrthographic = std::holds_alternative<ecs::Camera::Ortographic>(camera.projection);
	auto isCustom = std::holds_alternative<ecs::Camera::Custom>(camera.projection);
	auto options = std::array{"Perspective", "Orthographic", "Custom"};
	int currentItem = -1;

	if (isPerspective)
		currentItem = 0;
	else if (isOrthographic)
		currentItem = 1;
	else if (isCustom)
		currentItem = 2;

	if (ImGui::Combo("Projection", &currentItem, &options[0], options.size()))
	{
		if (currentItem == 0)
			camera.projection = ecs::Camera::Perspective{};
		else if (currentItem == 1)
			camera.projection = ecs::Camera::Ortographic{};
		else if (currentItem == 2)
			camera.projection = ecs::Camera::Custom{};
	}

	if (currentItem == 0)
	{
		auto& proj = std::get<ecs::Camera::Perspective>(camera.projection);
		ImGui::SliderFloat("Field of View", &proj.fieldOfView, 15.0f, 120.0f, "%.0f");
		ImGui::InputFloat("Aspect Ratio", &proj.aspectRatio);
		ImGui::InputFloat("Near", &proj.near);
		ImGui::InputFloat("Far", &proj.far);
	}
	else if (currentItem == 1)
	{
		auto& proj = std::get<ecs::Camera::Ortographic>(camera.projection);
		ImGui::InputFloat("Aspect Ratio", &proj.aspectRatio);
		ImGui::InputFloat("Near", &proj.near);
		ImGui::InputFloat("Far", &proj.far);
	}
	else if (currentItem == 2)
	{
		auto& proj = std::get<ecs::Camera::Custom>(camera.projection);
		ImGui::InputFloat("Near", &proj.near);
		ImGui::InputFloat("Far", &proj.far);
	}
}

bool sceneHierarchy(const entt::registry& registry, entt::entity& selectedEntity)
{
	const auto view = registry.view<const ecs::Name>();
	bool hasSelectedEntity = false;

	for (entt::entity entity : view)
	{
		const auto& name = view.get<const ecs::Name>(entity);
		ImGuiTreeNodeFlags flags = (selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;

		bool opened =  ImGui::TreeNodeEx(reinterpret_cast<void*>(entity), flags, "%s", name.value.c_str());
		if (ImGui::IsItemClicked()) {
			selectedEntity = entity;
		}

		if (opened)
			ImGui::TreePop();
	}

	return hasSelectedEntity;
}

int inputTextCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		auto* str = reinterpret_cast<std::string*>(data->UserData);
		str->resize(data->BufTextLen);
		data->Buf = str->data();
	}

	return 1;
};

void inputText(const std::string_view label, std::string& value)
{
	ImGui::InputText(label.data(), value.data(), value.size() + 1, ImGuiInputTextFlags_CallbackResize,
					 &inputTextCallback, &value);
}

} // namespace spatial::ui