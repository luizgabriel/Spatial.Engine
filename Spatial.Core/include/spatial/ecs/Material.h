#pragma once

#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

namespace tags
{

struct IsMaterial
{
	constexpr static auto typeName = "tag_is_material";
};

} // namespace tags

struct MaterialInstance
{
	constexpr static auto typeName = "material_instance";

	struct Scissor
	{
		uint32_t left{};
		uint32_t bottom{};
		uint32_t width{};
		uint32_t height{};
	};

	Scissor scissor{};

	static void changeMaterialSource(Registry& registry, entt::entity materialInstance, Entity newMaterialSource);
};

} // namespace spatial::ecs