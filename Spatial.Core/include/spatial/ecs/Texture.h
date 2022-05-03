#pragma once

namespace spatial::ecs::tags
{

struct IsImageTexture
{
	constexpr static auto typeName = "tag_is_image_texture";
};

struct IsCubeMapTexture
{
	constexpr static auto typeName = "tag_is_cubemap_texture";
};

} // namespace spatial::ecs