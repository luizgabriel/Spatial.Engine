#pragma once

#include <spatial/render/Resources.h>
#include <spatial/assets/ResourcesLoader.h>
#include <spatial/ecs/Components.h>
#include <filament/Engine.h>
#include <vector>
#include <unordered_map>

namespace spatial::ecs
{

class MaterialRegistry
{
  private:
	filament::Engine& mEngine;
	const assets::ResourcesLoader& mResourcesLoader;

	std::size_t mMaterialInstancesLastIdx;
	std::unordered_map<std::uint32_t, spatial::Material> mLoadedMaterials;
	std::vector<spatial::MaterialInstance> mMaterialInstances;

  public:
	MaterialRegistry(filament::Engine& engine, const assets::ResourcesLoader& resourcesLoader);

	std::size_t load(const std::string_view resourcePath);

	template <typename Callback>
	std::size_t load(const std::string_view resourcePath, Callback callback)
	{
		auto materialInstanceId = load(resourcePath);
		callback(mMaterialInstances[materialInstanceId].ref());
		return materialInstanceId;
	}

	spatial::MaterialInstance& at(std::size_t materialId)
	{
		return mMaterialInstances.at(materialId);
	}

	auto& getEngine()
	{
		return mEngine;
	}
};

} // namespace spatial::ecs
