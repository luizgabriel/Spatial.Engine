#pragma once

#include <fmt/format.h>
#include <imgui.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/ecs/SceneNode.h>
#include <spatial/ecs/Transform.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Mesh.h>

namespace spatial::editor
{

void instancesTreeView(ecs::Registry& registry, ecs::Entity& selectedActor);

bool inputText(const std::string_view label, std::string& value);

bool directionInput(const std::string_view label, math::float3& v, float size = 100.0f,
					 std::uint32_t color = 0x22ff2200);

void componentInput(ecs::Transform& transform);

void componentInput(ecs::PerspectiveCamera& camera);

void componentInput(ecs::OrthographicCamera& camera);

void componentInput(ecs::CustomCamera& camera);

void componentInput(ecs::PointLight& light);

void componentInput(ecs::DirectionalLight& light);

void componentInput(ecs::SpotLight& light);

void componentInput(ecs::SunLight& light);

void componentInput(ecs::Mesh& mesh);

void componentInput(ecs::MeshRenderer& mesh);

bool buttonInput(const std::string_view label, float& value, float resetValue = .0f, float speed = .1f,
				 float min = .0f, float max = .0f, const std::string_view format = "%.2f");

bool vec2Input(const std::string_view label, math::float2& v, float resetValue = .0f, float speed = .1f,
			   float min = .0f, float max = .0f, const std::string_view format = "%.2f");

bool vec3Input(const std::string_view label, math::float3& v, float resetValue = .0f, float speed = .1f,
			   float min = .0f, float max = .0f, const std::string_view format = "%.2f");

bool vec4Input(const std::string_view label, math::float4& v, float resetValue = .0f, float speed = .1f,
			   float min = .0f, float max = .0f, const std::string_view format = "%.2f");


template <typename Component>
void componentGroup(const std::string_view componentName, ecs::EntityHandle& entity, bool defaultOpen = true)
{
	if (entity.has<Component>())
	{
		auto& component = entity.get<Component>();
		bool openedHeader = true;
		if (ImGui::CollapsingHeader(componentName.data(), &openedHeader,
									defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None))
		{
			ImGui::Indent();

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::PushID(componentName.data());
			componentInput(component);
			ImGui::PopID();

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Unindent();
		}

		if (!openedHeader)
		{
			entity.remove<Component>();
		}
	}
}

} // namespace spatial::editor