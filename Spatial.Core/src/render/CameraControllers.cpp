#include <spatial/render/CameraControllers.h>

using namespace filament::math;
namespace fl = filament;

namespace spatial
{

constexpr auto center = float2{0.5f, 0.5f};
constexpr auto up = float3{.0f, 1.0f, .0f};

filament::math::float3 toDirection(const float2& rot)
{
	const auto direction = float3{cos(rot.x) * cos(rot.y), sin(rot.y), sin(rot.x) * cos(rot.y)};
	return normalize(direction);
}

void SimpleCameraView::onMouseMoved(const float2& mousePos, float sensitivity)
{
	const auto delta = center - mousePos;
	rotation.x += delta.x * pi<float> * -sensitivity;
	rotation.y = std::clamp(rotation.y + delta.y * pi<float> * sensitivity, -halfPi<float>, halfPi<float>);
}

void SimpleCameraView::onUpdate(fl::Camera& camera, const float3& delta)
{
	const auto direction = toDirection(rotation);
	position += direction * delta.x;
	position += cross(direction, up) * delta.y;
	position += up * delta.z;

	camera.lookAt(position, position + direction, up);
}

} // namespace spatial