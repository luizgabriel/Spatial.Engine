#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Relation.h>

namespace spatial::ecs
{

Entity Mesh::addPart(Registry& registry, Entity meshEntity, MeshPart part)
{
	auto partEntity = registry.createEntity();
	registry.addComponent<ecs::MeshPart>(partEntity, part);
	ecs::Parent::addChild(registry, meshEntity, partEntity);

	return partEntity;
}

std::vector<Entity> Mesh::getParts(const Registry& registry, Entity meshEntity)
{
	return ecs::Parent::getChildren(registry, meshEntity);
}

size_t Mesh::getPartsCount(const Registry& registry, Entity meshEntity)
{
	return ecs::Parent::getChildrenCount(registry, meshEntity);
}

size_t MeshInstance::getPrimitivesCount(const Registry& registry, Entity meshInstanceEntity)
{
	if (!registry.hasComponent<ecs::Parent>(meshInstanceEntity))
		return 0;

	return ecs::Parent::getChildrenCount(registry, meshInstanceEntity);
}

Entity MeshInstance::addMaterial(Registry& registry, Entity meshInstanceEntity, Entity materialInstanceEntity)
{
	auto& meshInstance = registry.getOrAddComponent<ecs::MeshInstance>(meshInstanceEntity);

	if (!registry.isValid(materialInstanceEntity))
		materialInstanceEntity = meshInstance.defaultMaterialInstance;

	return addMaterial(registry, meshInstanceEntity, materialInstanceEntity,
					   getPrimitivesCount(registry, meshInstanceEntity));
}

Entity MeshInstance::addMaterial(Registry& registry, Entity meshInstanceEntity, Entity materialInstanceEntity,
								 size_t primitiveIndex)
{
	return ecs::Builder::create(registry)
		.asMeshPrimitive()
		.withMeshInstance(meshInstanceEntity)
		.withPrimitiveIndex(primitiveIndex)
		.withMaterial(materialInstanceEntity);
}

std::vector<Entity> MeshInstance::getPrimitives(const Registry& registry, Entity meshInstance)
{
	return ecs::Parent::getChildren(registry, meshInstance);
}

size_t MeshInstance::destroyPrimitives(Registry& registry, Entity meshInstanceEntity)
{
	return ecs::Parent::destroyChildren(registry, meshInstanceEntity);
}

void MeshPrimitive::changeMeshInstance(Registry& registry, Entity meshPrimitiveEntity, Entity meshInstanceEntity)
{
	ecs::Child::changeParent(registry, meshPrimitiveEntity, meshInstanceEntity);
}

} // namespace spatial::ecs