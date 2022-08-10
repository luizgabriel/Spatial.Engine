#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Scene.h>
#include <spatial/ecs/Texture.h>

namespace spatial::ecs
{

void Scene::addAttachment(Registry& registry, Entity sceneView, Entity attachmentEntity)
{
	ecs::Parent::addChild(registry, sceneView, attachmentEntity);
}

std::vector<Entity> Scene::getAttachments(const Registry& registry, Entity sceneView)
{
	return ecs::Parent::getChildren(registry, sceneView);
}

void Scene::resizeAttachments(Registry& registry, Entity sceneViewEntity)
{
	const auto& sceneView = registry.getComponent<const ecs::Scene>(sceneViewEntity);
	auto attachments = getAttachments(registry, sceneViewEntity);
	for (auto attachmentEntity : attachments)
	{
		const auto& attachment = registry.getComponent<const ecs::AttachmentTexture>(attachmentEntity);
		auto& texture = registry.getComponent<ecs::AttachmentTexture>(attachmentEntity);
		texture.size = sceneView.size;

		registry.removeComponent<ecs::tags::IsResourceLoaded>(attachmentEntity);
	}
}

} // namespace spatial::ecs