#include <spatial/render/CameraControllers.h>
#include <spatial/input/Mouse.h>

using namespace filament::math;

namespace spatial
{

constexpr auto center = float2{0.5f, 0.5f};

void incrementCameraRotation(float2& rotation, float sensitivity, const float2& mousePos)
{
	const auto delta = center - mousePos;
	rotation.x += delta.x * pi<float> * -sensitivity;
	rotation.y = std::clamp(rotation.y + delta.y * pi<float> * sensitivity, -halfPi<float>, halfPi<float>);
}

filament::math::float3 toDirection(const filament::math::float2& rot)
{
	const auto direction = filament::math::float3{cos(rot.x) * cos(rot.y),
												  sin(rot.y),
												  sin(rot.x) * cos(rot.y)};
	return normalize(direction);
}

void SimpleCameraView::update(const filament::math::float2& mousePos, float sensitivity)
{
	incrementCameraRotation(rotation, sensitivity, Mouse::position());
}

} // namespace spatial