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

void RegistryCollection::reserve(size_t capacity)
{
	mRegistries.reserve(capacity);
}

} // namespace spatial::ecs