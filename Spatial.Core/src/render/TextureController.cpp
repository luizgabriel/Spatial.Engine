#include <spatial/core/Logger.h>
#include <spatial/ecs/Resource.h>
#include <spatial/ecs/Texture.h>
#include <spatial/render/Resources.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/render/TextureController.h>

namespace spatial::render
{

render::SharedTexture createTexture(filament::Engine& engine, const ecs::RuntimeTexture& runtimeTexture)
{
	return toShared(createTexture(engine, runtimeTexture.pixels, runtimeTexture.width));
}

render::SharedTexture createTexture(filament::Engine& engine, const ecs::ResourceData& resourceData)
{
	return toShared(createKtxTexture(engine, resourceData.data.data(), resourceData.data.size()));
}

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
			registry.addOrReplaceComponent(entity, createTexture(engine, runtimeTexture));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry
		.getEntities<const ecs::ResourceData, ecs::tags::IsCubeMapTexture>(
			ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::ResourceData& resource) {
			registry.addOrReplaceComponent(entity, createTexture(engine, resource));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry
		.getEntities<const ecs::ResourceData, ecs::tags::IsImageTexture>(
			ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::ResourceData& resource) {
			registry.addOrReplaceComponent(entity,
										   toShared(createTexture(engine, resource.data.data(), resource.data.size())));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry
		.getEntities<const ecs::ResourceData, ecs::tags::IsIrradianceValues>(
			ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::ResourceData& resource) {
			registry.addOrReplaceComponent(entity, parseShFile(resource.data.data(), resource.data.size()));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});
}

} // namespace spatial::render