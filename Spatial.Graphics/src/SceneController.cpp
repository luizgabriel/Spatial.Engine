#include <spatial/ecs/Name.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Scene.h>
#include <spatial/ecs/Texture.h>
#include <spatial/graphics/Entity.h>
#include <spatial/graphics/Resources.h>
#include <spatial/graphics/SceneController.h>

namespace spatial::graphics
{

inline filament::View::BlendMode toFilament(ecs::Scene::BlendMode blendMode)
{
	switch (blendMode)
	{
	case ecs::Scene::BlendMode::Opaque:
		return filament::BlendMode::OPAQUE;
	case ecs::Scene::BlendMode::Translucent:
		return filament::BlendMode::TRANSLUCENT;
	}
}

inline filament::RenderTarget::AttachmentPoint toFilament(ecs::AttachmentTexture::Type attachmentType)
{
	switch (attachmentType)
	{
	case ecs::AttachmentTexture::Type::Color:
		return filament::RenderTarget::AttachmentPoint::COLOR;
	case ecs::AttachmentTexture::Type::Depth:
		return filament::RenderTarget::AttachmentPoint::DEPTH;
	}
}

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
		registry.getEntities<const SharedScene>().each([&](const auto& scene) {
			auto contains = scene->hasEntity(renderEntity.get());
			if (!contains)
			{
				scene->addEntity(renderEntity.get());
			}
		});
	});

	registry.insertComponent<ecs::tags::AddedToScene>(view.begin(), view.end());
}

void SceneController::renderViews(filament::Renderer& renderer, const ecs::Registry& registry)
{
	registry
		.getEntities<const SharedView>()	//
		.each([&](const SharedView& view) { //
			renderer.render(view.get());
		});
}

void SceneController::createScenes(filament::Engine& engine, ecs::Registry& registry)
{
	auto sceneViews = registry.getEntities<const ecs::Scene>(ecs::ExcludeComponents<SharedScene>);
	if (sceneViews.size_hint() > 0)
		registry.removeComponentFromEntities<ecs::tags::AddedToScene>();

	sceneViews.each([&](ecs::Entity entity, const auto&) {
		auto scene = toShared(createScene(engine));
		auto view = toShared(createView(engine));
		view->setScene(scene.get());

		registry.addComponent(entity, std::move(scene));
		registry.addComponent(entity, std::move(view));
	});

	registry
		.getEntities<ecs::tags::IsRenderedToTarget, const ecs::Scene, const SharedView>(
			ecs::ExcludeComponents<SharedRenderTarget>)
		.each([&](ecs::Entity entity, const auto&, const auto& view) {
			auto builder = filament::RenderTarget::Builder();
			auto attachments = ecs::Scene::getAttachments(registry, entity);
			if (attachments.size() < 2)
				return;

			for (auto attachmentEntity : attachments)
			{
				const auto& attachment = registry.getComponent<const ecs::AttachmentTexture>(attachmentEntity);
				const auto* texture = registry.tryGetComponent<const SharedTexture>(attachmentEntity);
				auto flAttachmentType = toFilament(attachment.type);

				builder = builder.texture(flAttachmentType, texture != nullptr ? texture->get() : nullptr);
			}

			auto renderTarget = toShared(createRenderTarget(engine, builder));
			view->setRenderTarget(renderTarget.get());

			registry.addComponent(entity, std::move(renderTarget));
		});
}

void SceneController::updateScenes(const ecs::Registry& registry)
{
	registry.getEntities<const ecs::Scene, const SharedScene>().each([&](const auto& sceneView, const auto& scene) {
		auto* indirectLight = registry.tryGetComponent<const SharedIndirectLight>(sceneView.indirectLight);
		scene->setIndirectLight(indirectLight != nullptr ? indirectLight->get() : nullptr);
	});

	registry.getEntities<const ecs::Scene, const SharedView>().each([&](const auto& sceneView, const auto& view) {
		const auto* camera = registry.tryGetComponent<const SharedCamera>(sceneView.camera);
		auto blendMode = toFilament(sceneView.blendMode);

		view->setViewport({0, 0, sceneView.size.x, sceneView.size.y});
		view->setCamera(camera != nullptr ? camera->get()->getInstance() : nullptr);
		view->setBlendMode(blendMode);
		view->setShadowingEnabled(sceneView.isShadowingEnabled);
		view->setPostProcessingEnabled(sceneView.isPostProcessingEnabled);
	});
}

} // namespace spatial::graphics