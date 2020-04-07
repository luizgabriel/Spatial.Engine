#pragma once

#include <math/vec2.h>
#include <spatial/common/Key.h>
#include <bitset>

namespace spatial
{

constexpr int keysCount = static_cast<size_t>(Key::Count);

class InputState
{
private:
	filament::math::float2 m_mousePosition;
	std::bitset<keysCount> m_keyPressed;
	std::bitset<keysCount> m_keyReleased;

public:
	InputState();

	void set(Key key, KeyAction action);

	void setPressed(Key key);

	void setReleased(Key key);

	void reset(Key key);

	void reset();

	void setMousePosition(filament::math::float2 position);

	bool released(Key key) const { return m_keyReleased.test(static_cast<size_t>(key)); }

	bool pressed(Key key) const { return m_keyPressed.test(static_cast<size_t>(key)); }

	bool combined(Key alt1, Key key) const { return pressed(alt1) && released(key); }

	bool combined(Key alt1, Key alt2, Key key) const
	{
		return pressed(alt1) && pressed(alt2) && released(key);
	}

	bool combined(Key alt1, Key alt2, Key alt3, Key key) const
	{
		return pressed(alt1) && pressed(alt2) && pressed(alt3) && released(key);
	}

	bool combined(Key alt1, Key alt2, Key alt3, Key alt4, Key key) const
	{
		return pressed(alt1) && pressed(alt2) && pressed(alt3) && pressed(alt4) && released(key);
	}

	filament::math::float2 getMousePosition() const { return m_mousePosition; }
};

} // namespace spatial::input