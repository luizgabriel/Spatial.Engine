#include <spatial/ecs/Resource.h>
#include <spatial/graphics/MaterialController.h>
#include <spatial/graphics/SkyboxResources.h>

namespace spatial::graphics
{

void MaterialController::loadMaterials(filament::Engine& engine, ecs::Registry& registry)
{
	registry
		.getEntities<const ecs::FileSystemResourceData, ecs::tags::IsMaterial>(
			ecs::Exclude<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::FileSystemResourceData& resource) {
			auto material = toShared(createMaterial(engine, resource.data.data(), resource.data.size()));

			registry.addOrReplaceComponent<SharedMaterial>(entity, std::move(material));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});
}

void MaterialController::createMaterialInstances(filament::Engine& engine, ecs::Registry& registry)
{
	registry.getEntities<const ecs::MaterialInstance>(ecs::Exclude<SharedMaterialInstance>)
		.each([&](ecs::Entity entity, const auto& materialInstance) {
			const auto materialEntity = materialInstance.materialSource;

			if (!registry.hasAllComponents<SharedMaterial>(materialEntity))
				return;

			auto& material = registry.getComponent<const SharedMaterial>(materialEntity);
			registry.addComponent<SharedMaterialInstance>(entity, toShared(createMaterialInstance(engine, material)));
		});
}

void MaterialController::updateMaterialInstances(const ecs::Registry& registry)
{
	registry.getEntities<const ecs::MaterialInstance, const SharedMaterialInstance>().each(
		[&](const auto& data, const SharedMaterialInstance& materialInstance) {
			materialInstance->setScissor(data.scissor.left, data.scissor.bottom, data.scissor.width,
										 data.scissor.height);
		});
}

} // namespace spatial::graphics