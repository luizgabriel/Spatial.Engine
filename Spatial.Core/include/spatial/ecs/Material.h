#pragma once

#include <spatial/resources/Resource.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/EntityHandle.h>

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

}

struct PrecompiledMaterial
{
	constexpr static auto typeName = "precompiled_material";

	Resource<FilaMat> resource{};

	static EntityConstHandle find(const ecs::Registry& registry, const std::filesystem::path& resourcePath);
	static EntityHandle findOrCreate(ecs::Registry& registry, const std::filesystem::path& resourcePath);
};

struct MaterialInstance
{
	constexpr static auto typeName = "material_instance";

	Entity materialEntity;
};

} // namespace spatial::ecs