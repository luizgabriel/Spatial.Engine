#include <spatial/render/RegistryRenderingSystem.h>

namespace spatial::render
{

RegistryRenderingSystem::RegistryRenderingSystem(filament::Engine& engine, FileSystem& fileSystem)
	: mEngine{engine},
	  mOnPublishRegistry{},
	  mSceneController{mEngine},
	  mTransformController{mEngine},
	  mCameraController{mEngine},
	  mLightController{mEngine},
	  mMeshController{mEngine, fileSystem},
	  mIndirectLightController{mEngine, fileSystem},
	  mMaterialController{mEngine, fileSystem}
{
}

void RegistryRenderingSystem::onStartFrame(float)
{
	auto registries = getPublishedRegistries();
	registries.each([this](auto& registry) {
		mMeshController.onStartFrame(registry);
	});
}

void RegistryRenderingSystem::onUpdateFrame(float)
{
	auto registries = getPublishedRegistries();
	registries.each([this](auto& registry) {
		mSceneController.onUpdateFrame(registry);
		mTransformController.onUpdateFrame(registry);
		mCameraController.onUpdateFrame(registry);
		mLightController.onUpdateFrame(registry);
		mIndirectLightController.onUpdateFrame(registry);
		mMaterialController.onUpdateFrame(registry);
		mMeshController.onUpdateFrame(registry);
	});
}

void RegistryRenderingSystem::onRender(filament::Renderer& renderer) const
{
	auto registries = getPublishedRegistries();
	registries.each([&](const ecs::Registry& registry) {
		registry.getEntities<const render::TextureView>().each(
			[&](const auto& textureView) {
				renderer.render(textureView.getView().get());
			});
	});
}

ecs::RegistryCollection RegistryRenderingSystem::getPublishedRegistries() const
{
	auto registries = ecs::RegistryCollection{};
	mOnPublishRegistry(registries);

	return registries;
}

} // namespace spatial::render