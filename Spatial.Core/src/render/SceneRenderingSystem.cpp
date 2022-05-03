#include <spatial/render/CameraController.h>
#include <spatial/render/IndirectLightController.h>
#include <spatial/render/LightController.h>
#include <spatial/render/MaterialController.h>
#include <spatial/render/MeshController.h>
#include <spatial/render/RegistryRenderingSystem.h>
#include <spatial/render/SceneController.h>
#include <spatial/render/TransformController.h>

namespace spatial::render
{

RegistryRenderingSystem::RegistryRenderingSystem(filament::Engine& engine, FileSystem& fileSystem)
	: mEngine{engine}, mFileSystem{fileSystem}, mOnPublishRegistry{}, mIndirectLightController{mEngine, mFileSystem}
{
}

void RegistryRenderingSystem::onRender(filament::Renderer& renderer)
{
	auto registries = getPublishedRegistries();
	auto& engine = *renderer.getEngine();

	registries.each([&](ecs::Registry& registry) {
		SceneController{engine}.onStartFrame(registry);
		MeshController{engine, mFileSystem}.onStartFrame(registry);

		SceneController{engine}.onUpdateFrame(registry);
		TransformController{engine}.onUpdateFrame(registry);
		CameraController{engine}.onUpdateFrame(registry);
		LightController{engine}.onUpdateFrame(registry);
		mIndirectLightController.onUpdateFrame(registry);

		MaterialLoaderController{engine, mFileSystem}.onUpdateFrame(registry);
		MeshController{engine, mFileSystem}.onUpdateFrame(registry);

		auto textureViews = registry.getEntities<const render::TextureView>();
		textureViews.each([&](const auto& textureView) { renderer.render(textureView.getView().get()); });
	});
}

ecs::RegistryCollection RegistryRenderingSystem::getPublishedRegistries() const
{
	auto registries = ecs::RegistryCollection{};
	registries.reserve(mOnPublishRegistry.getSize());
	mOnPublishRegistry.trigger(registries);

	return registries;
}

} // namespace spatial::render