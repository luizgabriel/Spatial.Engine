#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

Registry::Registry() : entt::registry{}
{
}

Entity Registry::createEntity()
{
	return create();
}

bool Registry::isValid(Entity entity) const noexcept
{
	return valid(entity);
}

size_t Registry::getEntitiesCount() const noexcept
{
	return size();
}

void Registry::destroyEntity(Entity entity)
{
	destroy(entity);
}

Registry::VersionType Registry::getVersion(Entity entity) noexcept
{
	return entt::registry::version(entity);
}

const ecs::Entity* Registry::getEntities() const
{
	return data();
}

ecs::Entity Registry::getLastDestroyedEntity() const
{
	return destroyed();
}

void Registry::destroyOrphans()
{
	orphans([this](const auto e) { destroyEntity(e); });
}

} // namespace spatial::ecs