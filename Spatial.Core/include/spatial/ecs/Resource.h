#pragma once

#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

namespace tags
{

struct IsResourceLoaded
{
};

} // namespace tags

struct Resource
{
	constexpr static auto typeName = "resource";

	std::filesystem::path relativePath;

	static Entity find(const Registry& registry, const std::filesystem::path& resource);

	static Entity findOrCreate(Registry& registry, const std::filesystem::path& resource);
};

struct ResourceError
{
	std::string errorMessage;
};

} // namespace spatial::ecs