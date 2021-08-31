#include <spatial/ecs/EntityHandle.h>
#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

Registry::Registry() : mRegistry{}
{
}

Entity Registry::createEntity()
{
	return mRegistry.create();
}

bool Registry::isValid(Entity entity) const noexcept
{
	return mRegistry.valid(entity);
}

size_t Registry::getEntitiesCount() const noexcept
{
	return mRegistry.size();
}

void Registry::destroy(Entity entity)
{
	mRegistry.destroy(entity);
}

Registry::VersionType Registry::getVersion(Entity entity) noexcept
{
	return entt::registry::version(entity);
}

} // namespace spatial