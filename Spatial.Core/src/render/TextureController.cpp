#include <spatial/render/SkyboxResources.h>
#include <boost/algorithm/string/predicate.hpp>
#include <spatial/ecs/Resource.h>
#include <spatial/ecs/Texture.h>
#include <spatial/render/Resources.h>
#include <spatial/render/TextureController.h>

using namespace boost::algorithm;

namespace spatial::render
{

TextureController::TextureController(filament::Engine& engine, FileSystem& fileSystem)
	: mEngine{engine}, mFileSystem{fileSystem}
{
}

void TextureController::onUpdateFrame(ecs::Registry& registry)
{
	registry
		.getEntities<const ecs::Resource, ecs::tags::IsImageTexture>(ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::Resource& resource) {
			if (resource.relativePath.empty())
				return;

			auto isPng = ends_with(resource.relativePath.c_str(), ".png");
			auto isJpg = ends_with(resource.relativePath.c_str(), ".jpg");

			if (!isPng && !isJpg)
				return;

			const auto data = mFileSystem.readBinary(resource.relativePath);
			auto texture = toShared(createTexture(mEngine, data.data(), data.size()));

			registry.addOrReplaceComponent<SharedTexture>(entity, std::move(texture));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry
		.getEntities<const ecs::Resource, ecs::tags::IsCubeMapTexture>(ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::Resource& resource) {
			if (resource.relativePath.empty())
				return;

			auto isKtx = ends_with(resource.relativePath.c_str(), ".ktx");

			if (!isKtx)
				return;

			const auto data = mFileSystem.readBinary(resource.relativePath);
			auto texture = toShared(createKtxTexture(mEngine, data.data(), data.size()));

			registry.addOrReplaceComponent<SharedTexture>(entity, std::move(texture));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry
		.getEntities<const ecs::Resource, ecs::tags::IsIrradianceValues>(ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::Resource& resource) {
			if (resource.relativePath.empty())
				return;

			auto isTxt = ends_with(resource.relativePath.c_str(), ".txt");

			if (!isTxt)
				return;

			const auto stream = mFileSystem.openReadStream(resource.relativePath);
			if (stream->bad())
				return;

			auto bands = bands_t{};
			*stream >> bands;

			registry.addOrReplaceComponent<bands_t>(entity, bands);
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});
}

} // namespace spatial::render