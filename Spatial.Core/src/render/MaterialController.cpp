#include <boost/algorithm/string/predicate.hpp>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Resource.h>
#include <spatial/ecs/Tags.h>
#include <spatial/render/MaterialController.h>
#include <spatial/render/SkyboxResources.h>

using namespace boost::algorithm;

namespace spatial::render
{

static auto gLogger = createDefaultLogger();

MaterialController::MaterialController(filament::Engine& engine, FileSystem& fileSystem)
	: mEngine{engine}, mFileSystem{fileSystem}, mJobQueue{}, mTextures{}
{
	mJobQueue.connect<LoadResourceEvent>(*this);
}

void MaterialController::load(uint32_t resourceId, Texture&& texture)
{
	mTextures.emplace(resourceId, std::move(texture));
}

void MaterialController::onStartFrame() const
{
	mJobQueue.update();
}

void MaterialController::onEvent(const LoadResourceEvent& event)
{
	if (event.texture.isEmpty())
		return;

	const auto isImageTexture =
		ends_with(event.texture.relativePath.c_str(), ".png") || ends_with(event.texture.relativePath.c_str(), ".jpg");

	if (isImageTexture)
	{
		const auto data = mFileSystem.readBinary(event.texture.relativePath.string());
		if (data.empty())
		{
			gLogger.warn("Image Texture not found: {}", event.texture.relativePath.string());
			return;
		}

		auto texture = render::createTexture(mEngine, data.data(), data.size());
		mTextures.emplace(event.texture.getId(), std::move(texture));
		return;
	}

	const auto isCubeMapTexture = ends_with(event.texture.relativePath.c_str(), ".ktx");

	if (isCubeMapTexture)
	{
		const auto data = mFileSystem.readBinary(event.texture.relativePath.string());
		if (data.empty())
		{
			gLogger.warn("CubeMap Texture not found: {}", event.texture.relativePath.string());
			return;
		}

		auto texture = render::createKtxTexture(mEngine, data.data(), data.size());
		mTextures.emplace(event.texture.getId(), std::move(texture));
	}
}

void MaterialController::onUpdateFrame(ecs::Registry& registry) const
{
	registry
		.getEntities<const ecs::Resource, ecs::tags::IsMaterial>(ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::Resource& resource) {
			if (resource.relativePath.empty() || !ends_with(resource.relativePath.c_str(), ".filamat"))
				return;

			const auto data = mFileSystem.readBinary(resource.relativePath.string());
			auto material = toShared(render::createMaterial(mEngine, data.data(), data.size()));

			registry.addOrReplaceComponent<SharedMaterial>(entity, material);
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
														  toShared(render::createMaterialInstance(mEngine, material)));
		});
}

const filament::Texture* MaterialController::findResource(const ResourcePath& resource)
{
	if (resource.isEmpty())
		return nullptr;

	auto it = mTextures.find(resource.getId());
	const auto* texture = (it != mTextures.end()) ? it->second.get() : nullptr;
	if (texture == nullptr)
		mJobQueue.enqueue(LoadResourceEvent{resource});

	return texture;
}

} // namespace spatial::render