#include <spatial/core/Logger.h>
#include <spatial/ecs/Resource.h>
#include <spatial/ecs/Texture.h>
#include <spatial/render/Resources.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/render/TextureController.h>

namespace spatial::render
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
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry
		.getEntities<const ecs::ResourceData, ecs::tags::IsCubeMapTexture>(
			ecs::ExcludeComponents<render::SharedTexture>)
		.each([&](ecs::Entity entity, const ecs::ResourceData& resource) {
			registry.addComponent(entity,
								  toShared(createKtxTexture(engine, resource.data.data(), resource.data.size())));
		});

	registry
		.getEntities<const ecs::ResourceData, ecs::tags::IsImageTexture>(ecs::ExcludeComponents<render::SharedTexture>)
		.each([&](ecs::Entity entity, const ecs::ResourceData& resource) {
			registry.addComponent(entity, toShared(createTexture(engine, resource.data.data(), resource.data.size())));
		});

	registry.getEntities<const ecs::ResourceData, ecs::tags::IsIrradianceValues>(ecs::ExcludeComponents<bands_t>)
		.each([&](ecs::Entity entity, const ecs::ResourceData& resource) {
			registry.addComponent<bands_t>(entity, parseShFile(resource.data.data(), resource.data.size()));
		});
}

} // namespace spatial::render