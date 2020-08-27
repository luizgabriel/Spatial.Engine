#include <math/mat4.h>
#include <math/vec3.h>
#include <spatial/ecs/Components.h>
#include <spatial/ecs/TransformSystem.h>

namespace spatial::ecs
{

TransformSystem::TransformSystem(filament::Engine& engine) : mTransformManager{engine.getTransformManager()}
{
}

void TransformSystem::onUpdate(entt::registry& registry)
{
	using namespace filament::math;

	const auto view = registry.view<const ecs::Transform, Renderable>();

	view.each([this, &view](const auto& transform, auto& renderable) {
		const auto transformEntity = mTransformManager.getInstance(renderable.entity.get());
		const auto translation = mat4f::translation(transform.position);
		const auto scale = mat4f::scaling(transform.scale);
		const auto rotateZ = mat4f::rotation(transform.rotation.z, float3{0, 0, 1});
		const auto rotateX = mat4f::rotation(transform.rotation.x, float3{1, 0, 0});
		const auto rotateY = mat4f::rotation(transform.rotation.y, float3{0, 1, 0});

		const auto model = translation * (rotateX * rotateY * rotateZ) * scale ;
		mTransformManager.setTransform(transformEntity, model);
	});
}

} // namespace spatial::ecs