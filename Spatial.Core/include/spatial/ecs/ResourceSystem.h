#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <spatial/assets/ResourcesLoader.h>
#include <filament/Engine.h>
#include <unordered_map>
#include <vector>

namespace spatial::ecs
{

template <typename ComponentType, typename ResourceType, ResourceType (*CreatorFunction)(filament::Engine&, const std::vector<char>&) >
class ResourceSystem
{
  public:
	ResourceSystem(filament::Engine& engine, const assets::ResourcesLoader& loader)
		: mEngine{engine}, mResourceLoader{loader}
	{
	}

	ResourceSystem(const ResourceSystem& other) = delete;
	ResourceSystem& operator =(const ResourceSystem& other) = delete;

	void onConstruct(entt::registry& registry, entt::entity entity)
	{
		const auto& component = registry.get<ComponentType>(entity);

		if (!mResourcesMap.contains(component.resourceFilePath)) {
			auto resourceData = mResourceLoader(component.resourceFilePath);
			if (resourceData) {
				auto resource = CreatorFunction(mEngine, resourceData.value());
				mResourcesMap.emplace(component.resourceFilePath, std::move(resource));
			} else {
				// Throw?
			}
		}
	}

	const ResourceType& get(const std::string& resourcePath) const
	{
		return mResourcesMap.at(resourcePath);
	}

	ResourceType& get(const std::string& resourcePath)
	{
		return mResourcesMap.at(resourcePath);
	}

  private:
	filament::Engine& mEngine;
	const assets::ResourcesLoader& mResourceLoader;

	std::unordered_map<std::string, ResourceType> mResourcesMap;
};

} // namespace spatial::ecs
