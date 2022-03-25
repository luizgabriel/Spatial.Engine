#include <fmt/format.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ecs/Relation.h>

namespace spatial::ecs
{

Entity Mesh::find(const Registry& registry, const std::filesystem::path& resource)
{
	auto view = registry.getEntities<const ecs::Mesh>();

	//TODO: Can optimize to log(n) search if meshes are sorted
	auto it = std::find_if(view.begin(), view.end(), [&](ecs::Entity entity) {
		auto& mesh = registry.getComponent<const ecs::Mesh>(entity);
		return mesh.resource.relativePath == resource;
	});

	if (it == view.end())
		return NullEntity;

	return *it;
}

Entity Mesh::findOrCreate(Registry& registry, const std::filesystem::path& resource)
{
	auto found = Mesh::find(registry, resource);
	if (registry.isValid(found))
		return found;

	return build(registry).asMesh().withResource(resource);
}

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
	auto child = build(registry).asMeshMaterial().withPrimitiveIndex(primitiveIndex).withMaterial(materialEntity);
	ecs::Parent::addChild(registry, meshEntity, child);

	return child;
}

} // namespace spatial::ecs