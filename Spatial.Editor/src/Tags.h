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

struct IsEditorWhiteTexture
{
	constexpr static auto typeName = "tag_is_editor_white_texture";
};

struct IsEditorBlackTexture
{
	constexpr static auto typeName = "tag_is_editor_black_texture";
};

} // namespace spatial::editor::tags