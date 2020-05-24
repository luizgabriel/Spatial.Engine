#include <spatial/render/MouseMovementCameraController.h>
#include <spatial/input/Mouse.h>

namespace spatial
{

template <typename AngleType>
constexpr AngleType eulerToRadian = AngleType{0.01745329251}; // PI / 180

constexpr auto center = math::float2{0.5f, 0.5f};

template <typename AngleType>
constexpr auto radians(AngleType angle) noexcept
{
	return angle * eulerToRadian<AngleType>;
}

MouseMovementCameraController::MouseMovementCameraController(float mouseSensitivity, float yaw, float pitch) noexcept
	: m_mouseSensitivity{mouseSensitivity}, m_rotation{yaw, pitch}, m_direction{.0f, .0f, .0f}
{
	onMouseMoved(center);
}

void MouseMovementCameraController::onMouseMoved(math::float2 normalizedMousePosition)
{
	auto delta = center - normalizedMousePosition;

	m_rotation.x += delta.x * -m_mouseSensitivity;
	m_rotation.y = std::clamp(m_rotation.y + delta.y * m_mouseSensitivity, -90.0f, 90.0f);

	updateDirections();
}

void MouseMovementCameraController::updateDirections()
{
	auto direction = math::float3{cos(radians(m_rotation.x)) * cos(radians(m_rotation.y)),
								  sin(radians(m_rotation.y)),
								  sin(radians(m_rotation.x)) * cos(radians(m_rotation.y))};

	m_direction = normalize(direction);
}

} // namespace spatial