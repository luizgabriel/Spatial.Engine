#include <spatial/render/CameraControllers.h>
#include <spatial/input/Mouse.h>
#include <spatial/input/Keyboard.h>

using namespace filament::math;

namespace spatial
{

constexpr auto center = float2{0.5f, 0.5f};
constexpr auto up = float3{.0f, 1.0f, .0f};

filament::math::float3 toDirection(const filament::math::float2& rot)
{
	const auto direction = filament::math::float3{cos(rot.x) * cos(rot.y),
												  sin(rot.y),
												  sin(rot.x) * cos(rot.y)};
	return normalize(direction);
}

void SimpleCameraView::onMouseMoved(const filament::math::float2& mousePos, float sensitivity)
{
    const auto delta = center - mousePos;
    rotation.x += delta.x * pi<float> * -sensitivity;
    rotation.y = std::clamp(rotation.y + delta.y * pi<float> * sensitivity, -halfPi<float>, halfPi<float>);
}

void SimpleCameraView::onUpdate(filament::Camera* camera, float delta)
{
    const auto direction = toDirection(rotation);
    position += direction * delta * Keyboard::axis(Key::W, Key::S);
    position += cross(direction, up) * delta * Keyboard::axis(Key::D, Key::A);

	camera->lookAt(position, position + direction, up);
}

} // namespace spatial