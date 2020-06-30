#pragma once

#include <spatial/common/Key.h>
#include <bitset>
#include <math/vec2.h>

namespace spatial
{

constexpr int keysCount = static_cast<size_t>(Key::Count);

class InputState
{
private:
	std::bitset<keysCount> mKeyPressed;
	std::bitset<keysCount> mKeyReleased;

	filament::math::float2 m_lastMousePosition;
	filament::math::float2 m_currentMousePosition;
	bool m_mouseWarpRequested;

public:
	InputState();

	void set(Key key, KeyAction action);

	void setPressed(Key key);

	void setReleased(Key key);

	void reset(Key key);

	void reset();

	void setMousePosition(filament::math::float2 position);

	[[nodiscard]] auto getCurrentPosition() const
	{
		return m_currentMousePosition;
	}

	[[nodiscard]] auto getLastPosition() const
	{
		return m_lastMousePosition;
	}

	[[nodiscard]] bool isMouseWarpRequested() const
	{
		return m_mouseWarpRequested;
	}

	void warpMouseInWindow(filament::math::float2 position);

	[[nodiscard]] bool released(Key key) const
	{
		return mKeyReleased.test(static_cast<size_t>(key));
	}

	[[nodiscard]] bool pressed(Key key) const
	{
		return mKeyPressed.test(static_cast<size_t>(key));
	}

	[[nodiscard]] bool combined(Key alt1, Key key) const
	{
		return pressed(alt1) && released(key);
	}

	[[nodiscard]] bool combined(Key alt1, Key alt2, Key key) const
	{
		return pressed(alt1) && pressed(alt2) && released(key);
	}

	[[nodiscard]] bool combined(Key alt1, Key alt2, Key alt3, Key key) const
	{
		return pressed(alt1) && pressed(alt2) && pressed(alt3) && released(key);
	}

	[[nodiscard]] bool combined(Key alt1, Key alt2, Key alt3, Key alt4, Key key) const
	{
		return pressed(alt1) && pressed(alt2) && pressed(alt3) && pressed(alt4) && released(key);
	}
};

} // namespace spatial