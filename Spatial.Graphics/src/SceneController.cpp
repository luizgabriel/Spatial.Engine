#include <spatial/ecs/Name.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Texture.h>
#include <spatial/ecs/View.h>
#include <spatial/graphics/Entity.h>
#include <spatial/graphics/Resources.h>
#include <spatial/graphics/SceneController.h>

namespace spatial::graphics
{

inline filament::View::BlendMode toFilament(ecs::View::BlendMode blendMode)
{
	switch (blendMode)
	{
	case ecs::View::BlendMode::Opaque:
		return filament::BlendMode::OPAQUE;
	case ecs::View::BlendMode::Translucent:
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
	registry.getEntities<ecs::tags::IsRenderable>(ecs::Exclude<Entity>).each([&](ecs::Entity entity) {
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
	auto view = registry.getEntities<Entity>(ecs::Exclude<ecs::tags::AddedToScene>);

	view.each([&](auto& renderEntity) {
		registry.getEntities<const SharedScene>().each([&](const auto& scene) {
			auto contains = scene->hasEntity(renderEntity.get());
			if (!contains)
				scene->addEntity(renderEntity.get());
		});
	});

	registry.insertComponent<ecs::tags::AddedToScene>(view.begin(), view.end());
}

void SceneController::renderViews(filament::Renderer& renderer, const ecs::Registry& registry)
{
	registry
		.getEntities<const ecs::View, const SharedView>() //
		.each([&](const auto& scene, const auto& view) {  //
			if (registry.hasComponent<SharedCamera>(scene.camera))
				renderer.render(view.get());
		});
}

filament::RenderTarget::Builder makeRenderTargetBuilder(const ecs::Registry& registry, const ecs::View& view)
{
	auto builder = filament::RenderTarget::Builder();
	auto attachments = std::array<ecs::Entity, 2>{view.colorAttachment, view.depthAttachment};

	for (auto attachmentEntity : attachments)
	{
		if (!registry.hasComponent<ecs::AttachmentTexture>(attachmentEntity))
			continue;

		const auto& attachment = registry.getComponent<const ecs::AttachmentTexture>(attachmentEntity);
		const auto* texture = registry.tryGetComponent<const SharedTexture>(attachmentEntity);
		auto flAttachmentType = toFilament(attachment.type);

		builder = builder.texture(flAttachmentType, texture != nullptr ? texture->get() : nullptr);
	}

	return builder;
}

void SceneController::createScenes(filament::Engine& engine, ecs::Registry& registry)
{
	auto sceneViews = registry.getEntities<const ecs::View>(ecs::Exclude<SharedScene>);
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
		.getEntities<ecs::tags::IsRenderedToTarget, const ecs::View, const SharedView>(ecs::Exclude<SharedRenderTarget>)
		.each([&](ecs::Entity entity, const auto& view, const auto& renderView) {
			auto renderTarget = toShared(createRenderTarget(engine, makeRenderTargetBuilder(registry, view)));
			renderView->setRenderTarget(renderTarget.get());

			registry.addComponent(entity, std::move(renderTarget));
		});
}

void SceneController::updateScenes(const ecs::Registry& registry)
{
	registry.getEntities<const ecs::View, const SharedScene>().each([&](const auto& sceneView, const auto& scene) {
		auto* indirectLight = registry.tryGetComponent<const SharedIndirectLight>(sceneView.indirectLight);
		scene->setIndirectLight(indirectLight != nullptr ? indirectLight->get() : nullptr);
	});

	registry.getEntities<const ecs::View, const SharedView>().each([&](const auto& scene, const auto& view) {
		const auto* camera = registry.tryGetComponent<const SharedCamera>(scene.camera);
		auto blendMode = toFilament(scene.blendMode);

		view->setViewport({0, 0, scene.size.x, scene.size.y});
		view->setCamera(camera ? camera->get()->getInstance() : nullptr);
		view->setBlendMode(blendMode);
		view->setShadowingEnabled(scene.isShadowingEnabled);
		view->setPostProcessingEnabled(scene.isPostProcessingEnabled);
	});
}

} // namespace spatial::graphics