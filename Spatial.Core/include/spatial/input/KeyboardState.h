#pragma once

#include <spatial/common/Key.h>
#include <bitset>

namespace spatial
{

constexpr int keysCount = static_cast<size_t>(Key::Count);

class KeyboardState
{
private:
	std::bitset<keysCount> m_keyPressed;
	std::bitset<keysCount> m_keyReleased;

public:
	KeyboardState();

	void set(Key key, KeyAction action);

	void setPressed(Key key);

	void setReleased(Key key);

	void reset(Key key);

	void reset();

	[[nodiscard]] bool released(Key key) const
	{
		return m_keyReleased.test(static_cast<size_t>(key));
	}

	[[nodiscard]] bool pressed(Key key) const
	{
		return m_keyPressed.test(static_cast<size_t>(key));
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