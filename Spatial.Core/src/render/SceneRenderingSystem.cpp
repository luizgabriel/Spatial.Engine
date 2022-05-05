#include <spatial/render/CameraController.h>
#include <spatial/render/LightController.h>
#include <spatial/render/MaterialController.h>
#include <spatial/render/MeshController.h>
#include <spatial/render/RegistryRenderingSystem.h>
#include <spatial/render/SceneController.h>
#include <spatial/render/TextureController.h>
#include <spatial/render/TextureView.h>
#include <spatial/render/TransformController.h>

namespace spatial::render
{

RegistryRenderingSystem::RegistryRenderingSystem(FileSystem& fileSystem) : mFileSystem{fileSystem}, mOnPublishRegistry{}
{
}

void RegistryRenderingSystem::onStartFrame(float)
{
	auto registries = getPublishedRegistries();

	registries.each([&](ecs::Registry& registry) {
		SceneController::cleanUpDestroyableEntities(registry);
		LightController::deleteLights(registry);
		CameraController::deleteCameras(registry);
	});
}

void RegistryRenderingSystem::onUpdateFrame(float)
{
	auto registries = getPublishedRegistries();

	registries.each([&](ecs::Registry& registry) {
		SceneController::updateScenes(registry);
		SceneController::organizeSceneRenderables(registry);
		TransformController::updateTransforms(registry);
		CameraController::updateCameras(registry);
		LightController::updateLights(registry);
		MeshController::updateMeshInstances(registry);
	});
}

void RegistryRenderingSystem::onRender(filament::Renderer& renderer)
{
	auto registries = getPublishedRegistries();
	auto& engine = *renderer.getEngine();

	registries.each([&](ecs::Registry& registry) {
		MeshController::loadMeshes(engine, mFileSystem, registry);
		TextureController::loadTextures(engine, mFileSystem, registry);
		MaterialController::loadMaterials(engine, mFileSystem, registry);

		SceneController::createScenes(engine, registry);
		SceneController::createRenderables(engine, registry);
		TransformController::createTransforms(engine, registry);
		CameraController::createCameras(engine, registry);
		LightController::createLights(engine, registry);
		MaterialController::createMaterialInstances(engine, registry);
		MeshController::createMeshInstances(engine, registry);

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