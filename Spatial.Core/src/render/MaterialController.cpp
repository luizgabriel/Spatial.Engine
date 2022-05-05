#include <spatial/core/Logger.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Resource.h>
#include <spatial/render/MaterialController.h>
#include <spatial/render/SkyboxResources.h>

namespace spatial::render
{

static auto gLogger = createDefaultLogger();

void MaterialController::loadMaterials(filament::Engine& engine, FileSystem& fileSystem, ecs::Registry& registry)
{
	registry
		.getEntities<const ecs::Resource, ecs::tags::IsMaterial>(ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::Resource& resource) {
			if (resource.relativePath.empty())
				return;

			const auto data = fileSystem.readBinary(resource.relativePath);
			if (data.empty())
			{
				gLogger.warn("Could not load material: {}", resource.relativePath);
				return;
			}

			auto material = toShared(render::createMaterial(engine, data.data(), data.size()));

			registry.addOrReplaceComponent<SharedMaterial>(entity, std::move(material));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});
}

void MaterialController::createMaterialInstances(filament::Engine& engine, ecs::Registry& registry)
{
	registry
		.getEntities<const ecs::tags::IsMaterialInstance, const ecs::Child>(
			ecs::ExcludeComponents<SharedMaterialInstance>)
		.each([&](ecs::Entity entity, const ecs::Child& child) {
			const auto materialEntity = child.parent;

			if (!registry.hasAllComponents<SharedMaterial>(materialEntity))
				return;

			auto& material = registry.getComponent<const SharedMaterial>(materialEntity);
			auto materialInstance = toShared(render::createMaterialInstance(engine, material));
			registry.addComponent<SharedMaterialInstance>(entity, std::move(materialInstance));
		});
}

} // namespace spatial::render