#include <spatial/ecs/Name.h>
#include <spatial/ecs/SceneView.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Resources.h>
#include <spatial/render/SceneController.h>
#include <spatial/render/TextureView.h>

namespace spatial::render
{

SceneController::SceneController(filament::Engine& engine) : mEngine{engine}
{
}

void SceneController::onUpdateFrame(ecs::Registry& registry)
{
	registry.getEntities<const ecs::SceneView>(ecs::ExcludeComponents<Scene>)
		.each([&](ecs::Entity entity, const auto&) { registry.addComponent(entity, createScene(mEngine)); });

	registry.getEntities<ecs::tags::IsRenderable>(ecs::ExcludeComponents<Entity>).each([&](ecs::Entity entity) {
		const auto& renderable = registry.addComponent(entity, createEntity(mEngine));

		registry.getEntities<Scene>().each([&](auto& scene) { scene->addEntity(renderable.get()); });
	});

	registry.getEntities<const ecs::SceneView, Scene>(ecs::ExcludeComponents<TextureView>)
		.each([&](ecs::Entity entity, const auto& sceneView, auto& scene) {
			auto* camera = registry.tryGetComponent<Camera>(sceneView.camera);
			if (!camera)
				return;

			auto& textureView = registry.addComponent<TextureView>(entity, mEngine, sceneView.size);
			textureView.setScene(scene.get());
			textureView.setCamera(camera->getInstance());
		});

	registry.getEntities<const ecs::SceneView, Scene>().each(
		[&](const ecs::SceneView& component, render::Scene& scene) {
			auto* indirectLight = registry.tryGetComponent<IndirectLight>(component.indirectLight);
			if (indirectLight)
				scene->setIndirectLight(indirectLight->get());
		});

	registry.getEntities<const ecs::SceneView, TextureView>().each(
		[&](ecs::Entity entity, const auto& sceneView, auto& textureView) {
			auto* camera = registry.tryGetComponent<Camera>(sceneView.camera);
			if (!camera)
				return;

			textureView.setCamera(camera->getInstance());
		});
}

} // namespace spatial::render