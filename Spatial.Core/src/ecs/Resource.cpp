#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Resource.h>

namespace spatial::ecs
{

Entity FileSystemResource::find(const Registry& registry, std::string_view resource)
{
	const auto view = registry.getEntities<const ecs::FileSystemResource>();

	// TODO: Can optimize to log(n) search if resources are always sorted
	auto it = std::find_if(view.begin(), view.end(), [&](ecs::Entity entity) {
		auto& res = registry.getComponent<const ecs::FileSystemResource>(entity);
		return res.relativePath == resource;
	});

	if (it == view.end())
		return NullEntity;

	return *it;
}

std::string FileSystemResource::stem() const
{
	if (relativePath.empty())
		return "";
	auto fileName = filename();
	auto firstDot = fileName.find_first_of(".");
	return firstDot == std::string::npos ? fileName : fileName.substr(0, firstDot);
}

std::string FileSystemResource::extension() const
{
	if (relativePath.empty())
		return "";
	auto fileName = filename();
	auto firstDot = fileName.find_first_of(".");
	return firstDot == std::string::npos ? "" : fileName.substr(firstDot);
}

std::string FileSystemResource::filename() const
{
	if (relativePath.empty())
		return "";
	auto lastSeparator = relativePath.find_last_of(SEPARATOR);
	return relativePath.substr(lastSeparator + 1);
}

bool FileSystemResource::exists(const Registry& registry, std::string_view resource)
{
	return registry.isValid(find(registry, resource));
}

Entity FileSystemResource::createEmpty(Registry& registry, std::string_view resource)
{
	return ecs::Builder::create(registry).asFileSystemResource().withPath(resource);
}

} // namespace spatial::ecs