#pragma once

#include <math/vec2.h>

namespace spatial
{

class MouseState
{
private:
	filament::math::float2 m_lastMousePosition;
	filament::math::float2 m_currentMousePosition;
	bool m_mouseWarpRequested;

public:
	MouseState();

	void set(filament::math::float2 position);

	auto getCurrentPosition()
	{
		return m_currentMousePosition;
	}
	auto getLastPosition()
	{
		return m_lastMousePosition;
	}
	bool isMouseWarpRequested()
	{
		return m_mouseWarpRequested;
	}

	void warpMouseInWindow(filament::math::float2 position);
	void reset();
};

} // namespace spatial