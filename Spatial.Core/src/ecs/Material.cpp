#include <filesystem>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/RegistryUtils.h>

namespace spatial::ecs
{

EntityConstHandle PrecompiledMaterial::find(const Registry& registry, const std::filesystem::path& resourcePath)
{
	auto view = registry.getEntities<const ecs::PrecompiledMaterial>();
	auto it = std::find_if(view.begin(), view.end(), [&](ecs::Entity entity) {
		auto& material = registry.getComponent<const ecs::PrecompiledMaterial>(entity);
		return material.resource.relativePath == resourcePath;
	});

	if (it == view.end())
		return handleOf(registry, NullEntity);

	return handleOf(registry, *it);
}

EntityHandle PrecompiledMaterial::findOrCreate(Registry& registry, const std::filesystem::path& resourcePath)
{
	auto found = PrecompiledMaterial::find(registry, resourcePath);
	if (found)
		return handleOf(registry, found);

	auto material = createEntity(registry);
	material.add(Name{resourcePath.filename()});
	material.add(PrecompiledMaterial{resourcePath});
	material.add<ecs::tags::IsMaterial>();
	material.add<ecs::tags::IsResource>();

	return material;
}

} // namespace spatial::ecs