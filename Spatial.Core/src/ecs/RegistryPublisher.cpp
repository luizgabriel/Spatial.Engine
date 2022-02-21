#include <spatial/ecs/RegistryCollection.h>

namespace spatial::ecs
{

void RegistryCollection::append(ecs::Registry& registry)
{
	mRegistries.emplace_back(&registry);
}

RegistryCollection::RegistryCollection() : mRegistries{}
{
}

} // namespace spatial::ecs