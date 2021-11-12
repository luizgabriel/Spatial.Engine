#include <spatial/ecs/EntityName.h>
#include <spatial/ecs/Relation.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Transform.h>
#include <spatial/render/TransformController.h>

namespace spatial::render
{

TransformController::TransformController(filament::Engine& engine)
	: mEngine{engine}
{
}

void TransformController::onUpdateFrame(ecs::Registry& registry) const
{
	auto view = registry.getEntities<const ecs::Transform, const Entity>();
	view.each([this](const auto& data, const auto& renderable) {
		auto transform = render::Transform{mEngine, renderable.get()};
		transform.setMatrix(data.getMatrix());
	});
}

} // namespace spatial::render