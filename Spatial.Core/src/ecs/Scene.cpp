#include <array>
#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Texture.h>
#include <spatial/ecs/View.h>

namespace spatial::ecs
{

void View::resizeAttachments(Registry& registry, Entity sceneViewEntity)
{
	const auto& sceneView = registry.getComponent<const ecs::View>(sceneViewEntity);
	auto attachments = std::array<ecs::Entity, 2>{sceneView.colorAttachment, sceneView.depthAttachment};

	for (auto attachmentEntity : attachments)
	{
		if (!registry.hasComponent<ecs::AttachmentTexture>(attachmentEntity))
			continue;

		const auto& attachment = registry.getComponent<const ecs::AttachmentTexture>(attachmentEntity);
		auto& texture = registry.getComponent<ecs::AttachmentTexture>(attachmentEntity);
		texture.size = sceneView.size;

		registry.removeComponent<ecs::tags::IsResourceLoaded>(attachmentEntity);
	}
}

} // namespace spatial::ecs