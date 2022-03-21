#include <spatial/ecs/Name.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/SceneView.h>
#include <spatial/ecs/Tags.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Resources.h>
#include <spatial/render/SceneController.h>
#include <spatial/render/TextureView.h>

namespace spatial::render
{

SceneController::SceneController(filament::Engine& engine) : mEngine{engine}
{
}

void SceneController::onStartFrame(ecs::Registry& registry)
{
	cleanUpDestroyableEntities(registry);
}

void SceneController::createRenderable(ecs::Registry& registry) const
{
	registry.getEntities<ecs::tags::IsRenderable>(ecs::ExcludeComponents<Entity>).each([&](ecs::Entity entity) {
		registry.addComponent(entity, createEntity(mEngine));
	});
}

void SceneController::createScenes(ecs::Registry& registry) const
{
	registry.getEntities<const ecs::SceneView>(ecs::ExcludeComponents<Scene>)
		.each([&](ecs::Entity entity, const auto&) {
			registry.addComponent(entity, createScene(mEngine));
			registry.removeComponentFromEntities<ecs::tags::AddedToScene>();
		});
}

void SceneController::onUpdateFrame(ecs::Registry& registry)
{
	createScenes(registry);
	createRenderable(registry);
	addToScene(registry);

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

void SceneController::cleanUpDestroyableEntities(ecs::Registry& registry)
{
	registry.getEntities<const ecs::tags::CanDestroy>().each([&](ecs::Entity entity) {
		if (registry.hasAnyComponent<ecs::Child>(entity))
			ecs::Child::remove(registry, entity);

		if (registry.hasAnyComponent<ecs::Parent>(entity))
		{
			auto children = ecs::Parent::getChildren(registry, entity);
			registry.removeComponent<ecs::Child>(children.begin(), children.end());
			registry.insertComponent<ecs::tags::CanDestroy>(children.begin(), children.end());
		}

		registry.destroyEntity(entity);
	});
}

void SceneController::addToScene(ecs::Registry& registry)
{
	auto view = registry.getEntities<Entity>(ecs::ExcludeComponents<ecs::tags::AddedToScene>);

	view.each([&](ecs::Entity entity, auto& renderEntity) {
		registry.getEntities<Scene>().each([&](auto& scene) {
			auto contains = scene->hasEntity(renderEntity.get());
			if (!contains)
				scene->addEntity(renderEntity.get());
		});
	});

	registry.insertComponent<ecs::tags::AddedToScene>(view.begin(), view.end());
}

} // namespace spatial::render