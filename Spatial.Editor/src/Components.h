#pragma once

#include <spatial/common/Math.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Registry.h>

namespace spatial::editor
{

namespace tags
{

struct IsEditorEntity
{
	constexpr static auto typeName = "tag_is_editor_entity";
};

struct IsEditorWhiteTexture
{
	constexpr static auto typeName = "tag_is_editor_white_texture";
};

struct IsEditorBlackTexture
{
	constexpr static auto typeName = "tag_is_editor_black_texture";
};

struct IsSelected
{
	constexpr static auto typeName = "tag_is_selected";

	static void replace(ecs::Registry& registry, ecs::Entity entity);
};

struct IsSelectedView
{
	constexpr static auto typeName = "tag_is_selected_view";
};

} // namespace tags

struct EditorCamera
{
	constexpr static auto typeName = "editor_camera";

	float sensitivity{.5F};
	float velocity{10.0F};

	math::vec2 viewOffset{};
	math::vec3 movementOffset{};

	static void replaceView(ecs::Registry& registry, ecs::Entity newControlledView);
	static void replaceCamera(ecs::Registry& registry, ecs::Entity newControlledEntity);
};

constexpr auto gSkyboxVertexLayout =
	ecs::VertexLayout{ecs::VertexLayoutMode::Interleaved,
					  {ecs::VertexDescription{ecs::VertexAttribute::Position, ecs::VertexAttributeType::Float3}}};

const auto gSkyboxVertexData = ecs::VertexData::create<math::vec3>({{-1.0f, -1.0f, 1.0f}, //
																	{3.0f, -1.0f, 1.0f},  //
																	{-1.0f, 3.0f, 1.0f}}, //
																   gSkyboxVertexLayout);

const auto gSkyboxIndexData = ecs::IndexData::create<uint16_t>({0, 1, 2});

} // namespace spatial::editor