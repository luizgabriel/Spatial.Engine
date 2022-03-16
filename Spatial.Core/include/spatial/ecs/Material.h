#pragma once

#include <spatial/resources/Resource.h>
#include <spatial/ecs/Registry.h>
#include <set>

namespace spatial::ecs
{

namespace tags
{

struct IsResource
{
	constexpr static auto typeName = "tag_is_resource";
};

struct IsMaterial
{
	constexpr static auto typeName = "tag_is_material";
};

struct IsMaterialLoaded
{
};

struct IsMaterialInstance
{
	constexpr static auto typeName = "tag_is_material_instance";
};

}

struct PrecompiledMaterial
{
	constexpr static auto typeName = "precompiled_material";

	Resource<FilaMat> resource{};

	static Entity find(const ecs::Registry& registry, const std::filesystem::path& resourcePath);
	static Entity findOrCreate(ecs::Registry& registry, const std::filesystem::path& resourcePath);
};

} // namespace spatial::ecs