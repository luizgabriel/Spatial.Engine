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

bool Registry::isValid(Entity entity) const
{
	return mRegistry.valid(entity);
}

size_t Registry::getEntitiesCount() const
{
	return mRegistry.size();
}

void Registry::destroy(Entity entity)
{
	mRegistry.destroy(entity);
}

Registry::VersionType Registry::getVersion(Entity entity) const
{
	return mRegistry.version(entity);
}

} // namespace spatial