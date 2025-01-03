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
	return this->storage<Entity>()->size();
}

void Registry::destroyEntity(Entity entity)
{
	destroy(entity);
}

const ecs::Entity* Registry::getEntities() const
{
	return this->storage<Entity>()->data();
}

} // namespace spatial::ecs