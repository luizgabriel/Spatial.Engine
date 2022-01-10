#include <fmt/format.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Relation.h>

namespace spatial::ecs
{

void Mesh::addMaterial(Registry& registry, Entity& meshEntity, Entity materialEntity)
{
	const auto& parent = registry.getOrAddComponent<ecs::Parent>(meshEntity);
	const auto childrenCount = parent.childrenCount;
	addMaterial(registry, meshEntity, materialEntity, childrenCount);
}

void Mesh::addMaterial(Registry& registry, Entity& meshEntity, Entity materialEntity, size_t primitiveIndex)
{
	auto& mesh = registry.getComponent<ecs::Mesh>(meshEntity);
	mesh.partsCount++;

	auto child = registry.createEntity();
	registry.addComponent<ecs::Name>(child, fmt::format("Primitive {}", primitiveIndex));
	registry.addComponent<ecs::MeshMaterial>(child, primitiveIndex, materialEntity);

	ecs::Parent::addChild(registry, meshEntity, child);
}

} // namespace spatial::ecs