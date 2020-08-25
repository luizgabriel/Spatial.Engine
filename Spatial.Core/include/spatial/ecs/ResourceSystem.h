#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <spatial/assets/ResourcesLoader.h>
#include <filament/Engine.h>
#include <unordered_map>

namespace spatial::ecs
{

template <typename ComponentType, typename ResourceType, auto CreatorFunction>
class ResourceSystem
{
  public:
	ResourceSystem(filament::Engine& engine, const assets::ResourcesLoader& loader)
		: mEngine{engine}, mResourceLoader{loader}
	{
	}

	void onConstruct(entt::registry& registry, entt::entity entity)
	{
		const auto& component = registry.get<ComponentType>(entity);

		if (!mResourcesMap.contains(component.resourceFilePath)) {
			auto resourceData = mResourceLoader(component.resourceFilePath);
			if (resourceData) {
				mResourcesMap.emplace(component.resourceFilePath, CreatorFunction(mEngine, resourceData.value()));
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
