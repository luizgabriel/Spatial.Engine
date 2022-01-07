#pragma once

#include <filesystem>
#include <spatial/resources/Resource.h>

namespace spatial::editor
{

struct LoadSceneEvent
{
	std::filesystem::path path;
};

struct SaveSceneEvent
{
	std::filesystem::path path;
};

struct ClearSceneEvent
{
};

struct OpenProjectEvent
{
	std::filesystem::path path;
};

template <ResourceType type>
struct LoadResourceEvent
{
	Resource<type> texture;
	LoadResourceEvent(Resource<type> resource) : texture{std::move(resource)}
	{
	}
};

} // namespace spatial::editor