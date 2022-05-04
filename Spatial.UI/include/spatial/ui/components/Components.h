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
#include <spatial/ecs/SceneView.h>
#include <spatial/ecs/Script.h>
#include <spatial/ecs/Tags.h>
#include <spatial/ecs/Transform.h>
#include <spatial/ui/components/Collapse.h>
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

void image(const filament::Texture* texture, math::float2 size = math::float2{0, 0},
		   math::float4 uv = math::float4{0, 0, 1, 1});

bool imageButton(const filament::Texture* texture, math::float2 size = math::float2{0, 0},
				 math::float4 uv = math::float4{0, 0, 1, 1});

template <typename Component, typename... Args>
struct ComponentInputImpl
{
	static constexpr auto sName = "Unknown";
	static bool draw(ecs::Registry&, ecs::Entity, Args...) = delete;
};

template <typename Component, typename... Args>
bool componentInput(ecs::Registry& registry, ecs::Entity entity, Args... args)
{
	return ComponentInputImpl<Component, Args...>::draw(registry, entity, std::forward<Args>(args)...);
}

template <typename Component, typename... Args>
bool componentCollapse(ecs::Registry& registry, ecs::Entity entity, Args... args)
{
	bool changed = false;

	if (registry.hasAllComponents<Component>(entity))
	{
		auto collapse = Collapse{ComponentInputImpl<Component, Args...>::sName};
		if (collapse.isOpen())
		{
			spacing(3);
			changed = componentInput<Component>(registry, entity, std::forward<Args>(args)...);
			spacing(3);
		}

		if (collapse.onClose()) {
			registry.removeComponent<Component>(entity);
			changed = true;
		}
	}

	return changed;
}

template <>
struct ComponentInputImpl<ecs::DirectionalLight>
{
	static constexpr auto sName = "Directional Light";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::PointLight>
{
	static constexpr auto sName = "Point Light";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::SpotLight>
{
	static constexpr auto sName = "Spot Light";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::SunLight>
{
	static constexpr auto sName = "Sun Light";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::IndirectLight>
{
	static constexpr auto sName = "Indirect Light";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::Resource>
{
	static constexpr auto sName = "Resource";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::MeshInstance>
{
	static constexpr auto sName = "Mesh Instance";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::MeshMaterial>
{
	static constexpr auto sName = "Mesh Material";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::Transform>
{
	static constexpr auto sName = "Transform";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::PerspectiveCamera>
{
	static constexpr auto sName = "Perspective Camera";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::OrthographicCamera>
{
	static constexpr auto sName = "Orthographic Camera";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::SceneView>
{
	static constexpr auto sName = "Scene Camera";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::CustomCamera>
{
	static constexpr auto sName = "Custom Camera";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::ScriptInfo>
{
	static constexpr auto sName = "Script Information";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::Parent>
{
	static constexpr auto sName = "Parent";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::Child>
{
	static constexpr auto sName = "Child";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

} // namespace spatial::ui