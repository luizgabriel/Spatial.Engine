#include <spatial/ecs/EntityName.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ecs/SceneNode.h>

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

} // namespace spatial::ecs