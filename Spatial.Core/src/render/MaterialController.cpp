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

MaterialLoaderController::MaterialLoaderController(filament::Engine& engine, FileSystem& fileSystem)
	: mEngine{engine}, mFileSystem{fileSystem}
{
}

void MaterialLoaderController::onUpdateFrame(ecs::Registry& registry) const
{
	registry
		.getEntities<const ecs::Resource, ecs::tags::IsMaterial>(ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::Resource& resource) {
			if (resource.relativePath.empty() || !ends_with(resource.relativePath.c_str(), ".filamat"))
				return;

			const auto data = mFileSystem.readBinary(resource.relativePath);
			auto material = toShared(render::createMaterial(mEngine, data.data(), data.size()));

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
														  toShared(render::createMaterialInstance(mEngine, material)));
		});
}

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
	if (event.texturePath.empty())
		return;

	const auto isImageTexture = ends_with(event.texturePath.c_str(), ".png") || ends_with(event.texturePath, ".jpg");
	const auto textureId = entt::hashed_string::value(event.texturePath.c_str());

	if (isImageTexture)
	{
		const auto data = mFileSystem.readBinary(event.texturePath);
		if (data.empty())
		{
			gLogger.warn("Image Texture not found: {}", event.texturePath);
			return;
		}

		auto texture = render::createTexture(mEngine, data.data(), data.size());
		mTextures.emplace(textureId, std::move(texture));
		return;
	}

	const auto isCubeMapTexture = ends_with(event.texturePath.c_str(), ".ktx");

	if (isCubeMapTexture)
	{
		const auto data = mFileSystem.readBinary(event.texturePath);
		if (data.empty())
		{
			gLogger.warn("CubeMap Texture not found: {}", event.texturePath);
			return;
		}

		auto texture = render::createKtxTexture(mEngine, data.data(), data.size());
		mTextures.emplace(textureId, std::move(texture));
	}
}

const filament::Texture* MaterialController::findResource(std::string_view resource)
{
	if (resource.empty())
		return nullptr;

	auto it = mTextures.find(entt::hashed_string::value(resource.data()));
	const auto* texture = (it != mTextures.end()) ? it->second.get() : nullptr;
	if (texture == nullptr)
		mJobQueue.enqueue(LoadResourceEvent{std::string{resource}});

	return texture;
}

} // namespace spatial::render