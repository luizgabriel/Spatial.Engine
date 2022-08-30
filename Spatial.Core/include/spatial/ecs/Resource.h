#pragma once

#include <spatial/ecs/Name.h>
#include <spatial/ecs/Registry.h>
#include <vector>

namespace spatial::ecs
{

namespace tags
{

struct IsResourceLoaded
{
};

struct IsResource
{
	constexpr static auto typeName = "is_resource";
};

} // namespace tags

struct FileSystemResource
{
	constexpr static auto typeName = "file_system_resource";
	constexpr static auto SEPARATOR = "/";

	std::string relativePath;

	[[nodiscard]] std::string stem() const;

	[[nodiscard]] std::string extension() const;

	[[nodiscard]] std::string filename() const;

	static bool exists(const Registry& registry, std::string_view resource);

	static Entity find(const Registry& registry, std::string_view resource);

	template <typename ResourceTypeFlag>
	static Entity create(Registry& registry, std::string_view resource)
	{
		auto entity = createEmpty(registry, resource);
		registry.template addComponent<ResourceTypeFlag>(entity);
		return entity;
	}

	template <typename ResourceTypeFlag>
	static Entity findOrCreate(Registry& registry, std::string_view resource)
	{
		auto entity = find(registry, resource);
		if (registry.isValid(entity))
			return entity;

		return create<ResourceTypeFlag>(registry, resource);
	}

  private:
	static Entity createEmpty(Registry& registry, std::string_view resource);
};

struct FileSystemResourceData
{
	std::vector<uint8_t> data;
};

struct ResourceError
{
	std::string errorMessage;
};

} // namespace spatial::ecs