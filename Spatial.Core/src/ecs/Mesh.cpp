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

	auto& mesh = registry.getComponent<ecs::Mesh>(meshEntity);

	auto child = registry.createEntity();
	registry.addComponent<ecs::Name>(child, fmt::format("Primitive {}", childrenCount));
	registry.addComponent<ecs::MeshMaterial>(child, childrenCount, materialEntity);

	ecs::Parent::addChild(registry, meshEntity, child);
	mesh.partsCount = std::max(mesh.partsCount, childrenCount + 1);
}

void Mesh::addMaterial(Registry& registry, Entity& meshEntity, Entity materialEntity, size_t primitiveIndex)
{
	const auto& parent = registry.getOrAddComponent<ecs::Parent>(meshEntity);
	const auto childrenCount = parent.childrenCount;

	auto& mesh = registry.getComponent<ecs::Mesh>(meshEntity);

	auto child = registry.createEntity();
	registry.addComponent<ecs::Name>(child, fmt::format("Primitive {}", childrenCount));
	registry.addComponent<ecs::MeshMaterial>(child, primitiveIndex, materialEntity);

	ecs::Parent::addChild(registry, meshEntity, child);
	mesh.partsCount = std::max(mesh.partsCount, childrenCount + 1);
}

} // namespace spatial::ecs