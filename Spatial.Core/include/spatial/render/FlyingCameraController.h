#pragma once

#include <math/vec2.h>

using namespace filament::math;

namespace spatial
{

class FlyingCameraController
{
private:
	float3 m_normalizedViewVector;
	float3 m_position;
	float3 m_viewPoint;
	float3 m_upVector;

	vec2<int32_t> m_windowCenterPosition;
	float m_mouseSensitivity;

	float m_moveSpeed;
	int m_forwardKeyCode;
	int m_backwardKeyCode;
	int m_strafeLeftKeyCode;
	int m_strafeRightKeyCode;
public:
};

}