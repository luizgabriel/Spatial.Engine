#include <fmt/format.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Relation.h>

namespace spatial::ecs
{

Entity Mesh::findByResource(const Registry& registry, const std::filesystem::path& resource)
{
	auto view = registry.getEntities<const ecs::Mesh>();
	auto it = std::find_if(view.begin(), view.end(), [&](ecs::Entity entity) {
		auto& mesh = registry.getComponent<const ecs::Mesh>(entity);
		return mesh.resource.relativePath == resource;
	});

	if (it == view.end())
		return ecs::NullEntity;

	return *it;
}

void MeshInstance::addMaterial(Registry& registry, Entity& meshEntity, Entity materialEntity)
{
	const auto& parent = registry.getOrAddComponent<ecs::Parent>(meshEntity);
	const auto childrenCount = parent.childrenCount;
	addMaterial(registry, meshEntity, materialEntity, childrenCount);
}

void MeshInstance::addMaterial(Registry& registry, Entity& meshEntity, Entity materialEntity, size_t primitiveIndex)
{
	auto& mesh = registry.getComponent<ecs::MeshInstance>(meshEntity);
	mesh.slice.count++;

	auto child = registry.createEntity();
	registry.addComponent<ecs::Name>(child, fmt::format("Primitive {}", primitiveIndex));
	registry.addComponent<ecs::MeshMaterial>(child, primitiveIndex, materialEntity);

	ecs::Parent::addChild(registry, meshEntity, child);
}

} // namespace spatial::ecs