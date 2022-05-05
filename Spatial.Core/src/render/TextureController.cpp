#include <spatial/core/Logger.h>
#include <spatial/ecs/Resource.h>
#include <spatial/ecs/Texture.h>
#include <spatial/render/Resources.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/render/TextureController.h>

namespace spatial::render
{

static auto gLogger = createDefaultLogger();

void TextureController::loadTextures(filament::Engine& engine, FileSystem& fileSystem, ecs::Registry& registry)
{
	registry
		.getEntities<const ecs::Resource, ecs::tags::IsImageTexture>(
			ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::Resource& resource) {
			if (resource.relativePath.empty())
				return;

			auto data = fileSystem.readBinary(resource.relativePath);
			if (data.empty())
			{
				gLogger.warn("Could not load texture: {}", resource.relativePath);
				return;
			}

			auto texture = toShared(createTexture(engine, data.data(), data.size()));

			registry.addOrReplaceComponent(entity, std::move(texture));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry
		.getEntities<const ecs::Resource, ecs::tags::IsCubeMapTexture>(
			ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::Resource& resource) {
			if (resource.relativePath.empty())
				return;

			auto data = fileSystem.readBinary(resource.relativePath);
			if (data.empty())
			{
				gLogger.warn("Could not load texture: {}", resource.relativePath);
				return;
			}

			auto texture = toShared(createKtxTexture(engine, data.data(), data.size()));

			registry.addOrReplaceComponent<SharedTexture>(entity, std::move(texture));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry
		.getEntities<const ecs::Resource, ecs::tags::IsIrradianceValues>(
			ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::Resource& resource) {
			if (resource.relativePath.empty())
				return;

			auto stream = fileSystem.openReadStream(resource.relativePath);
			if (stream->bad())
			{
				gLogger.warn("Could not load texture: {}", resource.relativePath);
				return;
			}

			auto bands = bands_t{};
			*stream >> bands;

			registry.addOrReplaceComponent<bands_t>(entity, bands);
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});
}

} // namespace spatial::render