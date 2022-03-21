#include <filesystem>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/RegistryUtils.h>

namespace spatial::ecs
{

Entity PrecompiledMaterial::find(const Registry& registry, const std::filesystem::path& resourcePath)
{
	auto view = registry.getEntities<const PrecompiledMaterial>();

	// TODO: Can optimize to log(n) search if precompiled-materials are sorted
	auto it = std::find_if(view.begin(), view.end(), [&](ecs::Entity entity) {
		auto& material = registry.getComponent<const PrecompiledMaterial>(entity);
		return material.resource.relativePath == resourcePath;
	});

	if (it == view.end())
		return NullEntity;

	return *it;
}

Entity PrecompiledMaterial::findOrCreate(Registry& registry, const std::filesystem::path& resourcePath)
{
	auto found = PrecompiledMaterial::find(registry, resourcePath);
	if (registry.isValid(found))
		return found;

	return build(registry).asPrecompiledMaterial().withResource(resourcePath);
}

} // namespace spatial::ecs