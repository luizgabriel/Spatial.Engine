#pragma once

namespace spatial::ecs::tags
{

struct IsMaterial
{
	constexpr static auto typeName = "tag_is_material";
};

struct IsResource
{
	constexpr static auto typeName = "tag_is_resource";
};

} // namespace spatial::ecs::tags