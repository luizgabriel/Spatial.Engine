#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ecs/Resource.h>

namespace spatial::ecs
{

Entity Resource::find(const Registry& registry, const std::filesystem::path& resource)
{
	const auto view = registry.getEntities<const ecs::Resource>();

	//TODO: Can optimize to log(n) search if meshes are sorted
	auto it = std::find_if(view.begin(), view.end(), [&](ecs::Entity entity) {
		auto& res = registry.getComponent<const ecs::Resource>(entity);
		return res.relativePath == resource;
	});

	if (it == view.end())
		return NullEntity;

	return *it;
}

Entity Resource::findOrCreate(Registry& registry, const std::filesystem::path& resource)
{
	auto found = Resource::find(registry, resource);
	if (registry.isValid(found))
		return found;

	return build(registry).asResource().withPath(resource);
}

}