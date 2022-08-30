#include <spatial/ecs/Relation.h>
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
			registry.removeComponent<ecs::FileSystemResourceData>(entity);
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});
}

void MaterialController::createMaterialInstances(filament::Engine& engine, ecs::Registry& registry)
{
	registry.getEntities<const ecs::MaterialInstance, const ecs::Child>(ecs::Exclude<SharedMaterialInstance>)
		.each([&](ecs::Entity entity, const auto&, const ecs::Child& child) {
			const auto materialEntity = child.parent;

			if (!registry.hasAllComponents<SharedMaterial>(materialEntity))
				return;

			auto& material = registry.getComponent<const SharedMaterial>(materialEntity);
			auto materialInstance = toShared(createMaterialInstance(engine, material));
			registry.addComponent<SharedMaterialInstance>(entity, std::move(materialInstance));
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