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

	std::string relativePath;

	[[nodiscard]] std::string stem() const;

	[[nodiscard]] std::string extension() const;

	[[nodiscard]] std::string filename() const;

	static bool exists(const Registry& registry, std::string_view resource);

	static Entity find(const Registry& registry, std::string_view resource);

	static Entity findOrCreate(Registry& registry, std::string_view resource);
};

struct ResourceError
{
	std::string errorMessage;
};

} // namespace spatial::ecs