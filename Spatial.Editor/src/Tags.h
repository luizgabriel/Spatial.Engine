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

struct IsSkyBox
{
	constexpr static auto typeName = "tag_is_skybox";
};

struct IsGridPlane
{
	constexpr static auto typeName = "tag_is_grid_plane";
};

} // namespace spatial::editor::tags