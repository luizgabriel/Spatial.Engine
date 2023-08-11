#pragma once

#include <spatial/common/AspectRatio.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Registry.h>
#include <string_view>
#include <vector>

namespace spatial::ecs
{

namespace tags
{

struct IsRenderable
{
	constexpr static auto typeName = "tag_is_renderable";
};

struct IsMainView
{
	constexpr static auto typeName = "tag_is_main_view";
};

struct IsRenderedToTarget
{
	constexpr static auto typeName = "tag_is_rendered_to_target";
};

struct AddedToScene
{
};

struct CanDestroy
{
};

} // namespace tags

struct View
{
	constexpr static auto typeName = "view";

	enum class BlendMode
	{
		Opaque,
		Translucent,
	};

	math::uvec2 size{gWideScreenAspectRatio.toVector() * 240.0};
	BlendMode blendMode{BlendMode::Opaque};
	bool isShadowingEnabled{true};
	bool isPostProcessingEnabled{true};
	Entity camera{ecs::NullEntity};
	Entity indirectLight{ecs::NullEntity};
	Entity colorAttachment{ecs::NullEntity};
	Entity depthAttachment{ecs::NullEntity};

	static void resizeAttachments(Registry& registry, Entity sceneViewEntity);
};

} // namespace spatial::ecs