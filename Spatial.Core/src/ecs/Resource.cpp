#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Resource.h>

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

std::string Resource::stem() const
{
	return "";
}

std::string Resource::extension() const
{
	auto firstDot = relativePath.find_first_of(".");
	return filename().substr(firstDot);
}

std::string Resource::filename() const
{
	auto lastSeparator = relativePath.find_last_of(SEPARATOR);
	return relativePath.substr(lastSeparator);
}

bool Resource::exists(const Registry& registry, std::string_view resource)
{
	return registry.isValid(find(registry, resource));
}

Entity Resource::createEmpty(Registry& registry, std::string_view resource)
{
	return ecs::Builder::create(registry).asResource().withPath(resource);
}

}