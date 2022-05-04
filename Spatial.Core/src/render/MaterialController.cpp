#include <boost/algorithm/string/predicate.hpp>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Resource.h>
#include <spatial/render/MaterialController.h>
#include <spatial/render/SkyboxResources.h>

using namespace boost::algorithm;

namespace spatial::render
{

void MaterialController::loadMaterials(filament::Engine& engine, FileSystem& fileSystem, ecs::Registry& registry)
{
	registry
		.getEntities<const ecs::Resource, ecs::tags::IsMaterial>(ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::Resource& resource) {
			if (resource.relativePath.empty())
				return;

			const auto data = fileSystem.readBinary(resource.relativePath);
			auto material = toShared(render::createMaterial(engine, data.data(), data.size()));

			registry.addOrReplaceComponent<SharedMaterial>(entity, std::move(material));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry
		.getEntities<const ecs::tags::IsMaterialInstance, const ecs::Child>(
			ecs::ExcludeComponents<SharedMaterialInstance>)
		.each([&](ecs::Entity entity, const ecs::Child& child) {
			const auto materialEntity = child.parent;

			if (!registry.hasAllComponents<SharedMaterial>(materialEntity))
				return;

			const auto material = registry.getComponent<const SharedMaterial>(materialEntity);
			registry.addComponent<SharedMaterialInstance>(entity,
														  toShared(render::createMaterialInstance(engine, material)));
		});
}

} // namespace spatial::render