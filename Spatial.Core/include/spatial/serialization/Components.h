#include <cereal/access.hpp>
#include <spatial/ecs/Components.h>
#include <spatial/serialization/Math.h>

namespace spatial
{

template <typename Archive>
void serialize(Archive& ar, const TransformComponent& transform)
{
	ar(transform.position, transform.scale, transform.rotation);
}

template <typename Archive>
void serialize(Archive& ar, PerspectiveProjection& projection)
{
	ar(projection.aspectRatio, projection.fieldOfView, projection.far, projection.near);
}

template <typename Archive>
void serialize(Archive& ar, OrthographicProjection& projection)
{
	ar(projection.bottom, projection.left, projection.right, projection.top);
}

template <typename Archive>
void serialize(Archive& ar, CustomProjection& projection)
{
	ar(projection.projectionMatrix, projection.near, projection.far);
}

template <typename Archive>
void serialize(Archive& ar, CameraProjection& projection)
{
	std::visit([&ar](const auto& proj) { ar(proj); }, projection);
}

template <typename Archive>
void serialize(Archive& ar, CameraComponent& camera)
{
	ar(camera.projection);
}

} // namespace spatial