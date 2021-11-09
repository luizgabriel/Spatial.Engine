#include <algorithm>
#include <spatial/ecs/EntityName.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ecs/Relation.h>

namespace spatial::ecs
{

EntityHandle createEntity(Registry& registry)
{
	return handleOf(registry, registry.createEntity());
}

EntityHandle createEntity(Registry& registry, std::string name)
{
	auto handle = handleOf(registry, registry.createEntity());
	handle.add<EntityName>(std::move(name));

	return handle;
}

EntityBuilder build(Registry& registry)
{
	return {registry};
}

Entity findEntityFromName(const Registry& registry, std::string_view entityName)
{
	auto view = registry.getEntities<const EntityName>();
	auto it = std::find_if(view.begin(), view.end(), [&](auto entity) {
		const auto& nameComponent = registry.getComponent<const EntityName>(entity);
		return nameComponent.name == entityName;
	});

	return it != view.end() ? *it : ecs::NullEntity;
}

Entity findEntityFromUUID(const Registry& registry, UUID uuid)
{
	auto view = registry.getEntities<const UUID>();
	auto it = std::find_if(view.begin(), view.end(), [&](auto entity) {
		const auto& uuidComponent = registry.getComponent<const UUID>(entity);
		return uuidComponent == uuid;
	});

	return it != view.end() ? *it : ecs::NullEntity;
}

std::optional<UUID> findUUIDFromEntityName(const ecs::Registry& registry, std::string_view name)
{
	const auto entity = findEntityFromName(registry, name);
	if (registry.isValid(entity)) {
		const auto* component = registry.getComponentIfExists<UUID>(entity);
		return component ? std::optional{*component} : std::nullopt;
	}

	return std::nullopt;
}

} // namespace spatial::ecs