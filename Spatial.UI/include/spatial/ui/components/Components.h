#pragma once

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
#include <spatial/ecs/Transform.h>
#include <spatial/ecs/Relation.h>
#include <spatial/render/TextureView.h>
#include "DirectionInput.h"
#include "Search.h"
#include "VectorInput.h"
#include "SceneView.h"

namespace spatial::ui
{

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
	static void draw(ecs::Registry&, ecs::Entity, Args&&...) = delete;
};

template <typename Component, typename... Args>
void componentInput(ecs::Registry& registry, ecs::Entity entity, Args&&... args)
{
	ComponentInputImpl<Component, Args...>::draw(registry, entity, std::forward<Args>(args)...);
}

template <>
struct ComponentInputImpl<ecs::DirectionalLight>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::PointLight>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::SpotLight>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::SunLight>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::IndirectLight>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::Mesh>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::MeshMaterial>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::Transform>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::PerspectiveCamera>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::OrthographicCamera>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::SceneView>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<ecs::CustomCamera>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

} // namespace spatial::ui