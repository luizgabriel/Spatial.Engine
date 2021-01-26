#pragma once

#include <fmt/format.h>
#include <imgui.h>
#include <spatial/common/Math.h>
#include <spatial/render/Camera.h>
#include <spatial/render/InstanceHandle.h>
#include <spatial/render/Light.h>
#include <spatial/render/Renderable.h>
#include <spatial/render/SceneNodeName.h>
#include <spatial/render/Stage.h>
#include <spatial/render/Transform.h>

namespace spatial::editor
{

void transformInput(Transform& transform, const std::string_view format);

void cameraInput(Camera& camera);

void instancesTreeView(Stage& registry, Instance& selectedActor);

bool inputText(const std::string_view label, std::string& value);

bool directionWidget(const std::string_view label, math::float3& v, float size = 100.0f,
					 std::uint32_t color = 0x22ff2200);

void lightInput(Light& light);

bool buttonInput(const std::string_view label, float& value, float resetValue = .0f, float speed = .1f,
				 float min = .0f, float max = .0f, const std::string_view format = "%.2f");

bool vec2Input(const std::string_view label, math::float2& v, float resetValue = .0f, float speed = .1f,
			   float min = .0f, float max = .0f, const std::string_view format = "%.2f");

bool vec3Input(const std::string_view label, math::float3& v, float resetValue = .0f, float speed = .1f,
			   float min = .0f, float max = .0f, const std::string_view format = "%.2f");

bool vec4Input(const std::string_view label, math::float4& v, float resetValue = .0f, float speed = .1f,
			   float min = .0f, float max = .0f, const std::string_view format = "%.2f");

template <typename Component>
void drawComponentView(InstanceHandle& instance, Component& component);

template <typename Component>
void componentView(const std::string_view componentName, InstanceHandle& instanceHandle, bool defaultOpen = true)
{
	if (instanceHandle.has<Component>())
	{
		auto& component = instanceHandle.get<Component>();
		bool openedHeader = true;
		if (ImGui::CollapsingHeader(componentName.data(), &openedHeader,
									defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None))
		{
			ImGui::Indent();

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::PushID(componentName.data());
			drawComponentView<Component>(instanceHandle, component);
			ImGui::PopID();

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Unindent();
		}

		if (!openedHeader)
		{
			instanceHandle.remove<Component>();
		}
	}
}

} // namespace spatial::editor