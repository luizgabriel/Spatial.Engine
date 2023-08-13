#pragma once

#include <spatial/common/Key.h>
#include <spatial/common/Math.h>

namespace spatial::desktop
{

class InputState
{
  public:
	InputState();

	void set(Key key, KeyAction action);

	void setPressed(Key key);

	void setReleased(Key key);

	void reset(Key key);

	void approximateMouseOffset(float delta);

	void reset();

	void setMousePosition(math::vec2 position);

	auto getMousePosition() const
	{
		return mCurrentMousePosition;
	}

	auto getMouseLastPosition() const
	{
		return mLastMousePosition;
	}

	auto getMouseOffset() const
	{
		return getMouseLastPosition() - getMousePosition();
	}

	bool released(Key key) const
	{
		return mKeyReleased.test(static_cast<size_t>(key));
	}

	bool pressed(Key key) const
	{
		return mKeyPressed.test(static_cast<size_t>(key));
	}

	template <typename... K>
	bool combined(K... keys) const
	{
		auto combination = KeyCombination{std::forward<K>(keys)...};
		return (mKeyPressed == combination.getPressedBitset()) && (mKeyReleased == combination.getReleasedBitset());
	}

	float axis(Key positive, Key negative) const
	{
		if (pressed(positive))
			return 1.0f;
		else if (pressed(negative))
			return -1.0f;
		else
			return .0f;
	}

  private:
	KeyBitSet mKeyPressed;
	KeyBitSet mKeyReleased;

	math::vec2 mLastMousePosition;
	math::vec2 mCurrentMousePosition;
};

} // namespace spatial::desktop