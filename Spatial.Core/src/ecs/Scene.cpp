#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Texture.h>
#include <spatial/ecs/View.h>

namespace spatial::ecs
{

void View::addAttachment(Registry& registry, Entity sceneView, Entity attachmentEntity)
{
	ecs::Parent::addChild(registry, sceneView, attachmentEntity);
}

std::vector<Entity> View::getAttachments(const Registry& registry, Entity sceneView)
{
	return ecs::Parent::getChildren(registry, sceneView);
}

void View::resizeAttachments(Registry& registry, Entity sceneViewEntity)
{
	const auto& sceneView = registry.getComponent<const ecs::View>(sceneViewEntity);
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