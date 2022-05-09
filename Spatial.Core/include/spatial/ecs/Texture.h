#pragma once

#include <spatial/common/Math.h>
#include <vector>

namespace spatial::ecs
{

namespace tags
{

struct IsImageTexture
{
	constexpr static auto typeName = "tag_is_image_texture";
};

struct IsCubeMapTexture
{
	constexpr static auto typeName = "tag_is_cubemap_texture";
};

struct IsIrradianceValues
{
	constexpr static auto typeName = "tag_is_irradiance_values";
};

} // namespace tags

struct DummyCubeMapTexture
{
	constexpr static auto typeName = "dummy_cubemap_texture";
};

struct RuntimeTexture
{
	constexpr static auto typeName = "color_texture";

	std::vector<uint32_t> pixels{0xFFFFFFFF};
	size_t width{1};
};

} // namespace spatial::ecs