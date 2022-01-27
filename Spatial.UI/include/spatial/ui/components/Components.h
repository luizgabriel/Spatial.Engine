#pragma once

#include "DirectionInput.h"
#include "SceneView.h"
#include "Search.h"
#include "VectorInput.h"
#include <filament/Texture.h>
#include <filesystem>
#include <fmt/format.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Transform.h>
#include <spatial/render/TextureView.h>
#include <spatial/ui/components/Collapse.h>

namespace spatial::ui
{

void spanToAvailWidth(float weight = 1.0f);

bool inputText(std::string_view label, std::string& value, std::string_view placeholder = "");

bool inputPath(std::string_view label, std::filesystem::path& path, std::string_view placeholder = "");

void spacing(std::uint32_t times = 1);

void separator(std::uint32_t spacing = 0);

void image(const filament::Texture& texture, math::float2 size = math::float2{0, 0},
		   math::float4 uv = math::float4{0, 0, 1, 1});

bool imageButton(const filament::Texture& texture, math::float2 size = math::float2{0, 0},
				 math::float4 uv = math::float4{0, 0, 1, 1});

template <typename Component, typename... Args>
struct ComponentInputImpl
{
	static constexpr auto sName = "Unknown";
	static void draw(ecs::Registry&, ecs::Entity, Args&&...) = delete;
};

template <typename Component, typename... Args>
void componentInput(ecs::Registry& registry, ecs::Entity entity, Args&&... args)
{
	ComponentInputImpl<Component, Args...>::draw(registry, entity, std::forward<Args>(args)...);
}

template <typename Component, typename... Args>
void componentCollapse(ecs::Registry& registry, ecs::Entity entity, Args&&... args)
{
	if (registry.hasAllComponents<Component>(entity))
	{
		auto collapse = Collapse{ComponentInputImpl<Component, Args...>::sName};
		if (collapse.isOpen())
		{
			spacing(3);
			componentInput<Component>(registry, entity, std::forward<Args>(args)...);
			spacing(3);
		}

		if (collapse.onClose())
			registry.removeComponent<Component>(entity);
	}
}

template <typename Component, typename... Args>
void componentCollapse(const std::string_view componentName, ecs::Registry& registry, ecs::Entity entity,
					   Args&&... args)
{
	if (registry.hasAllComponents<Component>(entity))
	{
		auto collapse = Collapse{componentName};
		if (collapse.isOpen())
		{
			spacing(3);
			componentInput<Component>(registry, entity, std::forward<Args>(args)...);
			spacing(3);
		}

		if (collapse.onClose())
			registry.removeComponent<Component>(entity);
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
struct ComponentInputImpl<ecs::IndirectLight>
{
	static constexpr auto sName = "Indirect Light";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};


template <>
struct ComponentInputImpl<ecs::Mesh>
{
	static constexpr auto sName = "Mesh";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};


template <>
struct ComponentInputImpl<ecs::DynamicMesh>
{
	static constexpr auto sName = "Dynamic Mesh";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};


template <>
struct ComponentInputImpl<ecs::MeshInstance>
{
	static constexpr auto sName = "Mesh";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::MeshMaterial>
{
	static constexpr auto sName = "Mesh Material";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
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
struct ComponentInputImpl<ecs::SceneView>
{
	static constexpr auto sName = "Scene Camera";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::CustomCamera>
{
	static constexpr auto sName = "Custom Camera";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

} // namespace spatial::ui