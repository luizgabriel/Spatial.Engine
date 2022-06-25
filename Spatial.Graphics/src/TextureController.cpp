#include "spatial/graphics/TextureResources.h"
#include <spatial/core/Logger.h>
#include <spatial/ecs/Resource.h>
#include <spatial/ecs/Texture.h>
#include <spatial/graphics/Resources.h>
#include <spatial/graphics/SkyboxResources.h>
#include <spatial/graphics/TextureController.h>

namespace spatial::graphics
{

void TextureController::loadTextures(filament::Engine& engine, ecs::Registry& registry)
{
	registry.getEntities<const ecs::DummyCubeMapTexture>(ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity) {
			auto texture = toShared(createDummyCubemap(engine));

			registry.addOrReplaceComponent(entity, std::move(texture));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry.getEntities<const ecs::RuntimeTexture>(ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::RuntimeTexture& runtimeTexture) {
			auto texture = toShared(createTexture(engine, runtimeTexture.pixels, runtimeTexture.width));
			registry.addOrReplaceComponent(entity, std::move(texture));
			registry.removeComponent<ecs::ResourceData>(entity);
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry
		.getEntities<const ecs::ResourceData, ecs::tags::IsCubeMapTexture>(
			ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::ResourceData& resource) {
			auto texture = toShared(createKtxTexture(engine, resource.data.data(), resource.data.size()));
			registry.addOrReplaceComponent(entity, std::move(texture));
			registry.removeComponent<ecs::ResourceData>(entity);
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry
		.getEntities<const ecs::ResourceData, ecs::tags::IsImageTexture>(
			ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::ResourceData& resource) {
			auto texture = toShared(createTexture(engine, resource.data.data(), resource.data.size()));
			registry.addOrReplaceComponent(entity, std::move(texture));
			registry.removeComponent<ecs::ResourceData>(entity);
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry
		.getEntities<const ecs::ResourceData, ecs::tags::IsIrradianceValues>(
			ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::ResourceData& resource) {
			registry.addOrReplaceComponent(entity, parseShFile(resource.data.data(), resource.data.size()));
			registry.removeComponent<ecs::ResourceData>(entity);
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});
}

} // namespace spatial::graphics