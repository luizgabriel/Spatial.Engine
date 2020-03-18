#pragma once

#include <math/vec2.h>
#include <spatial/common/Key.h>
#include <spatial/common/KeyAction.h>
#include <bitset>

namespace spatial::input
{

constexpr int keysCount = static_cast<size_t>(spatial::common::Key::Count);

class InputState
{
private:
	filament::math::float2 m_mousePosition;
	std::bitset<keysCount> m_keyPressed;
	std::bitset<keysCount> m_keyReleased;

public:
	InputState();

	void set(common::Key key, common::KeyAction action);

	void setPressed(common::Key key);

	void setReleased(common::Key key);

	void reset(common::Key key);

	void reset();

	void setMousePosition(filament::math::float2 position);

	bool released(common::Key key) const { return m_keyReleased.test(static_cast<size_t>(key)); }

	bool pressed(common::Key key) const { return m_keyPressed.test(static_cast<size_t>(key)); }

	bool combined(common::Key alt1, common::Key key) const { return pressed(alt1) && released(key); }

	bool combined(common::Key alt1, common::Key alt2, common::Key key) const
	{
		return pressed(alt1) && pressed(alt2) && released(key);
	}

	bool combined(common::Key alt1, common::Key alt2, common::Key alt3, common::Key key) const
	{
		return pressed(alt1) && pressed(alt2) && pressed(alt3) && released(key);
	}

	bool combined(common::Key alt1, common::Key alt2, common::Key alt3, common::Key alt4, common::Key key) const
	{
		return pressed(alt1) && pressed(alt2) && pressed(alt3) && pressed(alt4) && released(key);
	}

	filament::math::float2 getMousePosition() const { return m_mousePosition; }
};

} // namespace spatial::input