#include <spatial/ecs/Name.h>
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
	handle.add<Name>(std::move(name));

	return handle;
}

EntityBuilder build(Registry& registry)
{
	return EntityBuilder{registry};
}

} // namespace spatial::ecs