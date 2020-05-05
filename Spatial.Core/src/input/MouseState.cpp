#include <spatial/input/MouseState.h>

using namespace filament::math;

namespace spatial
{

MouseState::MouseState()
	: m_currentMousePosition{.0f, .0f},
	  m_lastMousePosition{.0f, .0f},
	  m_mouseWarpRequested{false}
{
}

void MouseState::set(float2 position)
{
	m_lastMousePosition = m_currentMousePosition;
	m_currentMousePosition = position;
}
void MouseState::reset()
{
	m_mouseWarpRequested = false;
}

void MouseState::warpMouseInWindow(float2 position) {
	m_mouseWarpRequested = true;
	set(position);
}

} // namespace spatial