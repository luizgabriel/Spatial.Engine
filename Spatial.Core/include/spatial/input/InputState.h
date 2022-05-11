#pragma once

#include <bitset>
#include <spatial/common/Key.h>
#include <spatial/common/Math.h>

namespace spatial::desktop
{

constexpr int keysCount = static_cast<size_t>(Key::Count);

class InputState
{
  public:
	using BitSet = std::bitset<keysCount>;

	InputState();

	void set(Key key, KeyAction action);

	void setPressed(Key key);

	void setReleased(Key key);

	void reset(Key key);

	void approximateMouseOffset(float delta);

	void reset();

	void setMousePosition(filament::math::float2 position);

	const auto& getMousePosition() const
	{
		return mCurrentMousePosition;
	}

	const auto& getMouseLastPosition() const
	{
		return mLastMousePosition;
	}

	auto getMouseOffset() const
	{
		return getMouseLastPosition() - getMousePosition();
	}

	bool released(Key key) const;

	bool pressed(Key key) const;

	bool combined(Key alt1, Key key) const;

	bool combined(Key alt1, Key alt2, Key key) const;

	bool combined(Key alt1, Key alt2, Key alt3, Key key) const;

	bool combined(Key alt1, Key alt2, Key alt3, Key alt4, Key key) const;

	float axis(Key positive, Key negative) const;

private:
	BitSet mKeyPressed;
	BitSet mKeyReleased;

	math::float2 mLastMousePosition;
	math::float2 mCurrentMousePosition;
};

} // namespace spatial::desktop