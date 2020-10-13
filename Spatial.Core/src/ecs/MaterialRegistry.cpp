#include <spatial/ecs/MaterialRegistry.h>
#include <spatial/render/ResourceLoaders.h>
#include <entt/core/hashed_string.hpp>

namespace spatial::ecs
{

MaterialRegistry::MaterialRegistry(filament::Engine& engine)
	: mEngine{engine},
	  mMaterialInstancesLastIdx{},
	  mLoadedMaterials{},
	  mMaterialInstances{}
{
}

std::size_t MaterialRegistry::load(const assets::ResourcesLoader& loader, const std::string_view resourcePath)
{
	auto hash = entt::hashed_string{resourcePath.data()}.value();

	if (!mLoadedMaterials.contains(hash)) {
		auto materialData = loader.load(resourcePath);
		if (!materialData) throw std::runtime_error("Mesh resource not found");

		mLoadedMaterials.emplace(hash, createMaterial(mEngine, materialData.value()));
	}

	auto& material = mLoadedMaterials.at(hash);
	auto materialInstanceIdx = mMaterialInstancesLastIdx++;
	mMaterialInstances.emplace_back(createMaterialInstance(mEngine, material.ref()));

	return materialInstanceIdx;
}

} // namespace spatial::ecs