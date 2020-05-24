#pragma once

#include <math/vec2.h>
#include <math/vec3.h>
#include <math/mat4.h>

namespace spatial
{

namespace math = filament::math;

class MouseMovementCameraController
{
private:
	float m_mouseSensitivity{};

	math::float2 m_rotation;
	math::float3 m_direction;

public:
	explicit MouseMovementCameraController(float mouseSensitivity, float yaw = .0f, float pitch = .0f) noexcept;

	void setMouseSensitivity(float mouseSensitivity) noexcept
	{
		m_mouseSensitivity = mouseSensitivity;
	}

	void setYaw(float yaw)
	{
		m_rotation.x = yaw;
	}

	void setPitch(float pitch)
	{
		m_rotation.y = pitch;
	}

	void updateDirections();

	[[nodiscard]] const auto& MouseMovementCameraController::getDirection() const
	{
		return m_direction;
	}

	[[nodiscard]] auto getYaw() const
	{
		return m_rotation.x;
	}

	[[nodiscard]] auto getPitch() const
	{
		return m_rotation.y;
	}

	void onMouseMoved(math::float2 normalizedMousePosition);
};

} // namespace spatial