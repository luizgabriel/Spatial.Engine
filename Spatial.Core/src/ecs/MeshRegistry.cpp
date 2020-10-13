#include <spatial/ecs/MeshRegistry.h>
#include <spatial/render/ResourceLoaders.h>
#include <entt/core/hashed_string.hpp>

namespace spatial::ecs
{

MeshRegistry::MeshRegistry(filament::Engine& engine)
	: mEngine{engine}, mLoadedMeshes{}
{
}

std::uint32_t MeshRegistry::load(const assets::ResourcesLoader& loader, const std::string_view resourcePath)
{
	auto hash = entt::hashed_string{resourcePath.data()}.value();

	if (mLoadedMeshes.contains(hash))
		return hash;

	auto meshData = loader.load(resourcePath);
	if (!meshData) throw std::runtime_error("Mesh resource not found");

	mLoadedMeshes.emplace(hash, createMesh(mEngine, meshData.value()));

	return hash;
}

}