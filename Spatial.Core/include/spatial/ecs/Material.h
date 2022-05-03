#pragma once

namespace spatial::ecs::tags
{

struct IsMaterial
{
	constexpr static auto typeName = "tag_is_material";
};

struct IsMaterialInstance
{
	constexpr static auto typeName = "tag_is_material_instance";
};

} // namespace spatial::ecs