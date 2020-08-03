#include "ShapeSystem.h"

#include <spatial/render/ResourceLoaders.h>

namespace fl = filament;

namespace spatial::ecs
{

ShapeSystem::ShapeSystem(filament::Engine& engine, const assets::ResourcesLoader& resources)
	: mEngine{engine},
	  mResources{resources},
	  mMeshDict{},
	  mMaterial{engine},
	  mMaterialInstance{engine}
{
}

void ShapeSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	const auto& shape = registry.get<Shape>(entity);

	auto renderable = utils::EntityManager::get().create();

	if (!mMeshDict.contains(shape.name)) {
		auto shapeData = mResources(shape.name).value();
		mMeshDict.emplace(shape.name, createMesh(mEngine, shapeData));
	}

	auto& mesh = mMeshDict.at(shape.name);
	mesh.build(renderable, mMaterialInstance.get());
	registry.emplace<Renderable>(entity, Entity{mEngine, renderable});
}


void ShapeSystem::setMaterial(const std::string& material)
{
	auto materialData = mResources(material).value();
	mMaterial = createMaterial(mEngine, materialData);
	mMaterialInstance = createMaterialInstance(mEngine, mMaterial.ref());
	mMaterialInstance->setParameter("baseColor", fl::math::float4{1.0f, 1.0f, 1.0f, 1.0f});
	mMaterialInstance->setParameter("metallic", .1f);
	mMaterialInstance->setParameter("roughness", .5f);
	mMaterialInstance->setParameter("reflectance", .1f);
}

} // namespace spatial::ecs