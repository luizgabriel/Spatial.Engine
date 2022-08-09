#include <spatial/ecs/Name.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/SceneView.h>
#include <spatial/ecs/Tags.h>
#include <spatial/graphics/Entity.h>
#include <spatial/graphics/Resources.h>
#include <spatial/graphics/SceneController.h>
#include <spatial/graphics/TextureView.h>

namespace spatial::graphics
{

void SceneController::createRenderables(filament::Engine& engine, ecs::Registry& registry)
{
	registry.getEntities<ecs::tags::IsRenderable>(ecs::ExcludeComponents<Entity>).each([&](ecs::Entity entity) {
		registry.addComponent(entity, createEntity(engine));
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

void SceneController::organizeSceneRenderables(ecs::Registry& registry)
{
	auto view = registry.getEntities<Entity>(ecs::ExcludeComponents<ecs::tags::AddedToScene>);

	view.each([&](auto& renderEntity) {
		registry.getEntities<SharedScene>().each([&](auto& scene) {
			auto contains = scene->hasEntity(renderEntity.get());
			if (!contains)
			{
				scene->addEntity(renderEntity.get());
			}
		});
	});

	registry.insertComponent<ecs::tags::AddedToScene>(view.begin(), view.end());
}

void SceneController::renderViews(filament::Renderer& renderer, ecs::Registry& registry)
{
	registry.getEntities<const TextureView>().each([&](const TextureView& textureView) {
		const auto& view = textureView.getView();
		renderer.render(view.get());
	});
}

void updateScene(const ecs::Registry& registry, const ecs::SceneView& component, TextureView& textureView)
{
	auto* indirectLight = registry.tryGetComponent<const SharedIndirectLight>(component.indirectLight);
	if (indirectLight)
		textureView.getScene()->setIndirectLight(indirectLight->get());

	if (registry.hasAllComponents<SharedCamera>(component.camera))
	{
		const auto& camera = registry.getComponent<const SharedCamera>(component.camera);
		textureView.setCamera(camera);
	}
}

void SceneController::createScenes(filament::Engine& engine, ecs::Registry& registry)
{
	auto sceneViews = registry.getEntities<const ecs::SceneView>(ecs::ExcludeComponents<SharedScene>);
	if (sceneViews.size_hint() > 0)
		registry.removeComponentFromEntities<ecs::tags::AddedToScene>();

	sceneViews.each(
		[&](ecs::Entity entity, const auto&) { registry.addComponent(entity, toShared(createScene(engine))); });

	registry.getEntities<const ecs::SceneView, const SharedScene>(ecs::ExcludeComponents<TextureView>)
		.each([&](ecs::Entity entity, const auto& sceneView, const auto& scene) {
			if (!registry.hasAllComponents<SharedCamera>(sceneView.camera))
				return;

			auto& textureView = registry.addComponent<TextureView>(entity, engine, sceneView.size);
			textureView.setScene(scene);

			updateScene(registry, sceneView, textureView);
		});
}

void SceneController::updateScenes(ecs::Registry& registry)
{
	registry.getEntities<const ecs::SceneView, TextureView>().each(
		[&](const auto& sceneView, auto& textureView) { updateScene(registry, sceneView, textureView); });
}

} // namespace spatial::graphics