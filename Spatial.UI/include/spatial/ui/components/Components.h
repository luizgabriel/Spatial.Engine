#pragma once

#include <filament/Texture.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Resource.h>
#include <spatial/ecs/Script.h>
#include <spatial/ecs/Transform.h>
#include <spatial/ecs/View.h>
#include <spatial/graphics/TextureUtils.h>
#include <spatial/ui/components/CollapsingHeader.h>
#include <spatial/ui/components/DirectionInput.h>
#include <spatial/ui/components/SceneView.h>
#include <spatial/ui/components/Search.h>
#include <spatial/ui/components/VectorInput.h>

namespace spatial::ui
{

void spanToAvailWidth(float weight = 1.0f);

bool inputText(std::string_view label, std::string& value, std::string_view placeholder = "");

bool inputPath(std::string_view label, std::string& value, std::string_view placeholder = "");

void spacing(std::uint32_t times = 1);

void separator(std::uint32_t spacing = 0);

void image(graphics::OptionalTexture texture, math::vec2 size = math::vec2{0, 0},
		   math::vec4 uv = math::vec4{0, 0, 1, 1});

bool imageButton(graphics::OptionalTexture texture, math::vec2 size = math::vec2{0, 0},
				 math::vec4 uv = math::vec4{0, 0, 1, 1});

template <typename Component, typename... Args>
struct ComponentInputImpl
{
	static constexpr auto sName = Component::typeName;
	static void draw(ecs::Registry&, ecs::Entity, Args...) = delete;
};

template <typename Component>
struct ComponentTagImpl
{
	static constexpr auto sName = Component::typeName;
	static void draw(const ecs::Registry&, ecs::Entity)
	{
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Button));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_Button));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100.0f);
		ImGui::Button(sName);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor(2);
	}
};

template <typename Component>
void componentTag(const ecs::Registry& registry, ecs::Entity entity)
{
	if (registry.template hasComponent<Component>(entity)) {
		ImGui::SameLine(.0, 2.0f);
		ComponentTagImpl<Component>::draw(registry, entity);
	}
}

template <typename Component, typename... Args>
void componentInput(ecs::Registry& registry, ecs::Entity entity, Args... args)
{
	ComponentInputImpl<Component, Args...>::draw(registry, entity, std::forward<Args>(args)...);
}

template <typename Component, typename... Args>
void componentCollapse(ecs::Registry& registry, ecs::Entity entity, Args... args)
{
	if (registry.hasAllComponents<Component>(entity))
	{
		auto collapse = CollapsingHeader{ComponentInputImpl<Component, Args...>::sName};
		if (collapse.isOpen())
		{
			spacing(3);
			componentInput<Component>(registry, entity, std::forward<Args>(args)...);
			spacing(3);
		}

		if (collapse.onClose())
		{
			registry.removeComponent<Component>(entity);
		}
	}
}

template <>
struct ComponentInputImpl<ecs::DirectionalLight>
{
	static constexpr auto sName = "Directional Light";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::PointLight>
{
	static constexpr auto sName = "Point Light";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::SpotLight>
{
	static constexpr auto sName = "Spot Light";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::SunLight>
{
	static constexpr auto sName = "Sun Light";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::IndirectLight, graphics::OptionalTexture>
{
	static constexpr auto sName = "Indirect Light";
	static void draw(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons);
};

template <>
struct ComponentInputImpl<ecs::FileSystemResource>
{
	static constexpr auto sName = "Resource";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::MeshInstance, graphics::OptionalTexture>
{
	static constexpr auto sName = "Mesh Instance";
	static void draw(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons);
};

template <>
struct ComponentInputImpl<ecs::MeshPrimitive, graphics::OptionalTexture>
{
	static constexpr auto sName = "Mesh Primitive";
	static void draw(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons);
};

template <>
struct ComponentInputImpl<ecs::Transform>
{
	static constexpr auto sName = "Transform";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::PerspectiveCamera>
{
	static constexpr auto sName = "Perspective Camera";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::OrthographicCamera>
{
	static constexpr auto sName = "Orthographic Camera";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::MaterialInstance, graphics::OptionalTexture>
{
	static constexpr auto sName = "Material Instance";
	static void draw(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons);
};

template <>
struct ComponentInputImpl<ecs::View, graphics::OptionalTexture>
{
	static constexpr auto sName = "Scene";
	static void draw(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons);
};

template <>
struct ComponentInputImpl<ecs::CustomCamera>
{
	static constexpr auto sName = "Custom Camera";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::ScriptModule>
{
	static constexpr auto sName = "Script Module";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::Parent, graphics::OptionalTexture>
{
	static constexpr auto sName = "Parent";
	static void draw(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons);
};

template <>
struct ComponentInputImpl<ecs::Child, graphics::OptionalTexture>
{
	static constexpr auto sName = "Child";
	static void draw(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons);
};

template <>
struct ComponentInputImpl<ecs::AttachmentTexture>
{
	static constexpr auto sName = "Attachment Texture";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::tags::IsImageTexture>
{
	static constexpr auto sName = "Image Texture";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

} // namespace spatial::ui