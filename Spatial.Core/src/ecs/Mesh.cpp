#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Relation.h>

namespace spatial::ecs
{

Entity MeshInstance::addMaterial(Registry& registry, Entity meshEntity, Entity materialEntity)
{
	auto& mesh = registry.getOrAddComponent<ecs::MeshInstance>(meshEntity);
	if (!registry.isValid(mesh.defaultMaterial)) {
		mesh.defaultMaterial = materialEntity;
		return ecs::NullEntity;
	}

	return addMaterial(registry, meshEntity, materialEntity, mesh.slice.count++);
}

Entity MeshInstance::addMaterial(Registry& registry, Entity meshEntity, Entity materialEntity, size_t primitiveIndex)
{
	auto child = ecs::Builder::create(registry).asMeshMaterial().withPrimitiveIndex(primitiveIndex).withMaterial(materialEntity);
	ecs::Parent::addChild(registry, meshEntity, child);

	return child;
}

} // namespace spatial::ecs