#include <spatial/resources/ResourceController.h>
#include <spatial/graphics/CameraController.h>
#include <spatial/graphics/LightController.h>
#include <spatial/graphics/MaterialController.h>
#include <spatial/graphics/MeshController.h>
#include <spatial/graphics/RegistryRenderingSystem.h>
#include <spatial/graphics/SceneController.h>
#include <spatial/graphics/TextureController.h>
#include <spatial/graphics/TextureView.h>
#include <spatial/graphics/TransformController.h>

namespace spatial::graphics
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
		ResourceController::loadResources(mFileSystem, registry);

		SceneController::updateScenes(registry);
		TransformController::updateTransforms(registry);
		CameraController::updateCameras(registry);
		LightController::updateLights(registry);
		MeshController::updateMeshInstances(registry);
		SceneController::organizeSceneRenderables(registry);
	});
}

void RegistryRenderingSystem::onRender(filament::Renderer& renderer)
{
	auto registries = getPublishedRegistries();
	auto& engine = *renderer.getEngine();

	registries.each([&](ecs::Registry& registry) {
		MeshController::loadMeshes(engine, registry);
		MaterialController::loadMaterials(engine, registry);
		TextureController::loadTextures(engine, registry);

		SceneController::createScenes(engine, registry);
		SceneController::createRenderables(engine, registry);
		TransformController::createTransforms(engine, registry);
		CameraController::createCameras(engine, registry);
		LightController::createLights(engine, registry);
		MaterialController::createMaterialInstances(engine, registry);
		MeshController::createMeshInstances(engine, registry);

		SceneController::renderViews(renderer, registry);
	});
}

ecs::RegistryCollection RegistryRenderingSystem::getPublishedRegistries() const
{
	auto registries = ecs::RegistryCollection{};
	registries.reserve(mOnPublishRegistry.getSize());
	mOnPublishRegistry.trigger(registries);

	return registries;
}

} // namespace spatial::graphics