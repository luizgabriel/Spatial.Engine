#pragma once

#include <spatial/render/Resources.h>
#include <spatial/render/Mesh.h>
#include <spatial/assets/ResourcesLoader.h>
#include <spatial/ecs/Components.h>
#include <string_view>

namespace spatial::ecs
{

class MeshRegistry
{
  private:
	filament::Engine& mEngine;
	std::unordered_map<std::uint32_t, spatial::Mesh> mLoadedMeshes;

  public:
	MeshRegistry(filament::Engine& engine);

	std::uint32_t load(const assets::ResourcesLoader& loader, const std::string_view resourcePath);

	spatial::Mesh& at(std::uint32_t meshId)
	{
		return mLoadedMeshes.at(meshId);
	}

	auto& getEngine()
	{
		return mEngine;
	}
};

} // namespace spatial::ecs
