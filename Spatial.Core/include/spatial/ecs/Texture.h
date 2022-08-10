#pragma once

#include <spatial/common/Math.h>
#include <spatial/common/AspectRatio.h>
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

struct IsDummyCubeMapTexture
{
	constexpr static auto typeName = "tag_is_dummy_cubemap_texture";
};

struct IsColorBufferTexture
{
	constexpr static auto typeName = "tag_is_color_buffer_texture";
};

struct IsDepthBufferTexture
{
	constexpr static auto typeName = "tag_is_depth_buffer_texture";
};

} // namespace tags

struct RuntimeTexture
{
	constexpr static auto typeName = "runtime_texture";

	std::vector<uint32_t> pixels{0xFFFFFFFF};
	uint32_t width{1};
};

struct AttachmentTexture
{
	constexpr static auto typeName = "attachment_texture";
	enum class Type {
		Color,
		Depth
	};

	Type type{Type::Color};
	math::uvec2 size{gUltraWideScreenAspectRatio.toVector() * 240.0};
};

} // namespace spatial::ecs