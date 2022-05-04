#include <spatial/ecs/EntityBuilder.h>
#include <spatial/ecs/Resource.h>
#include <filesystem>

namespace spatial::ecs
{

Entity Resource::find(const Registry& registry, std::string_view resource)
{
	const auto view = registry.getEntities<const ecs::Resource>();

	//TODO: Can optimize to log(n) search if resources are always sorted
	auto it = std::find_if(view.begin(), view.end(), [&](ecs::Entity entity) {
		auto& res = registry.getComponent<const ecs::Resource>(entity);
		return res.relativePath == resource;
	});

	if (it == view.end())
		return NullEntity;

	return *it;
}

Entity Resource::findOrCreate(Registry& registry, std::string_view resource)
{
	auto found = Resource::find(registry, resource);
	if (registry.isValid(found))
		return found;

	return ecs::EntityBuilder::create(registry).asResource().withPath(resource);
}

std::string Resource::stem() const
{
	return std::filesystem::path{relativePath}.stem().string();
}

std::string Resource::extension() const
{
	return std::filesystem::path{relativePath}.extension().string();
}

std::string Resource::filename() const
{
	return std::filesystem::path{relativePath}.filename().string();
}

}