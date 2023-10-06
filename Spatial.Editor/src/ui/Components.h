#pragma once

#include "../ecs/Components.h"
#include "../ecs/Materials.h"
#include <imgui.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Resource.h>
#include <spatial/ecs/Script.h>
#include <spatial/ecs/Texture.h>
#include <spatial/ecs/View.h>
#include <spatial/graphics/TextureUtils.h>
#include <spatial/ui/components/Basic.h>
#include <spatial/ui/components/CollapsingHeader.h>

namespace spatial::ui
{

enum class ComponentFilter
{
	All,
	Renderables,
	Lights,
	Cameras,
	MeshInstances,
	MaterialInstances,
	Views,
	Textures,
	Resources,
	Meshes,
	Materials,
	Scripts,
};

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
	if (registry.template hasComponent<Component>(entity))
	{
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
struct ComponentInputImpl<ecs::MeshPart>
{
	static constexpr auto sName = "SkyBox Material";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::tags::IsImageTexture>
{
	static constexpr auto sName = "Image Texture";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<editor::ColorMaterial>
{
	static constexpr auto sName = "Color Material";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<editor::EditorCamera>
{
	static constexpr auto sName = "Editor Camera";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<editor::GridMaterial>
{
	static constexpr auto sName = "Grid Camera";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<editor::SkyBoxMaterial, graphics::OptionalTexture>
{
	static constexpr auto sName = "SkyBox Material";
	static void draw(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons);
};

template <>
struct ComponentInputImpl<editor::StandardOpaqueMaterial, graphics::OptionalTexture>
{
	static constexpr auto sName = "Standard Opaque Material";
	static void draw(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons);
};

} // namespace spatial::ui