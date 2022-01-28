#pragma once

namespace spatial::editor::tags
{

struct IsEditorEntity
{
	constexpr static auto typeName = "tag_is_editor_entity";
};

struct IsEditorView
{
	constexpr static auto typeName = "tag_is_editor_view";
};

struct IsSkyBoxMeshInstance
{
	constexpr static auto typeName = "tag_is_skybox_mesh_instance";
};

struct IsGridPlane
{
	constexpr static auto typeName = "tag_is_grid_plane";
};

struct IsSkyBoxMeshResource
{
	constexpr static auto typeName = "tag_is_skybox_mesh_resource";
};

} // namespace spatial::editor::tags