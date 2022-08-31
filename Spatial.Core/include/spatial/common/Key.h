#pragma once

#include <bitset>
#include <string>

namespace spatial
{

enum class Key
{
	UnknownKey = 0, ///< Unhandled key

	A,		   ///< The A key
	B,		   ///< The B key
	C,		   ///< The C key
	D,		   ///< The D key
	E,		   ///< The E key
	F,		   ///< The F key
	G,		   ///< The G key
	H,		   ///< The H key
	I,		   ///< The I key
	J,		   ///< The J key
	K,		   ///< The K key
	L,		   ///< The L key
	M,		   ///< The M key
	N,		   ///< The N key
	O,		   ///< The O key
	P,		   ///< The P key
	Q,		   ///< The Q key
	R,		   ///< The R key
	S,		   ///< The S key
	T,		   ///< The T key
	U,		   ///< The U key
	V,		   ///< The V key
	W,		   ///< The W key
	X,		   ///< The X key
	Y,		   ///< The Y key
	Z,		   ///< The Z key
	Num0,	   ///< The 0 key
	Num1,	   ///< The 1 key
	Num2,	   ///< The 2 key
	Num3,	   ///< The 3 key
	Num4,	   ///< The 4 key
	Num5,	   ///< The 5 key
	Num6,	   ///< The 6 key
	Num7,	   ///< The 7 key
	Num8,	   ///< The 8 key
	Num9,	   ///< The 9 key
	Escape,	   ///< The Escape key
	LControl,  ///< The left Control key
	LShift,	   ///< The left Shift key
	LAlt,	   ///< The left Alt key
	System,	   ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
	RControl,  ///< The right Control key
	RShift,	   ///< The right Shift key
	RAlt,	   ///< The right Alt key
	Menu,	   ///< The Menu key
	LBracket,  ///< The [ key
	RBracket,  ///< The ] key
	Semicolon, ///< The ; key
	Comma,	   ///< The , key
	Period,	   ///< The . key
	Quote,	   ///< The ' key
	Slash,	   ///< The / key
	Backslash, ///< The \ key
	Tilde,	   ///< The ~ key
	Equal,	   ///< The = key
	Hyphen,	   ///< The - key (hyphen)
	Space,	   ///< The Space key
	Enter,	   ///< The Enter/Return key
	Backspace, ///< The Backspace key
	Tab,	   ///< The Tabulation key
	PageUp,	   ///< The Page up key
	PageDown,  ///< The Page down key
	End,	   ///< The End key
	Home,	   ///< The Home key
	Insert,	   ///< The Insert key
	Delete,	   ///< The Delete key
	Add,	   ///< The + key
	Subtract,  ///< The - key (minus, usually from numpad)
	Multiply,  ///< The * key
	Divide,	   ///< The / key
	Left,	   ///< Left arrow
	Right,	   ///< Right arrow
	Up,		   ///< Up arrow
	Down,	   ///< Down arrow
	Numpad0,   ///< The numpad 0 key
	Numpad1,   ///< The numpad 1 key
	Numpad2,   ///< The numpad 2 key
	Numpad3,   ///< The numpad 3 key
	Numpad4,   ///< The numpad 4 key
	Numpad5,   ///< The numpad 5 key
	Numpad6,   ///< The numpad 6 key
	Numpad7,   ///< The numpad 7 key
	Numpad8,   ///< The numpad 8 key
	Numpad9,   ///< The numpad 9 key
	F1,		   ///< The F1 key
	F2,		   ///< The F2 key
	F3,		   ///< The F3 key
	F4,		   ///< The F4 key
	F5,		   ///< The F5 key
	F6,		   ///< The F6 key
	F7,		   ///< The F7 key
	F8,		   ///< The F8 key
	F9,		   ///< The F9 key
	F10,	   ///< The F10 key
	F11,	   ///< The F11 key
	F12,	   ///< The F12 key
	F13,	   ///< The F13 key
	F14,	   ///< The F14 key
	F15,	   ///< The F15 key
	Pause,	   ///< The Pause key

	MouseLeft,
	MouseRight,
	MouseMiddle,

	GamepadA,
	GamepadB,

	Count ///< Keep last -- the total number of keyboard keys
};

enum class KeyAction
{
	UnknownAction,
	Pressed,
	Released,
};

constexpr int gKeysCount = static_cast<size_t>(Key::Count);

using KeyBitSet = std::bitset<gKeysCount>;

template <typename... K>
KeyBitSet bitsetOf(K... key)
{
	auto bit = KeyBitSet{};
	(bit.set(static_cast<size_t>(key)), ...);
	return bit;
}

template <size_t N>
struct KeyCombination
{
	std::array<Key, N> keys{};

	template <typename... K>
	KeyCombination(K... keys) : keys{keys...}
	{
	}

	template <size_t I = 0>
	constexpr KeyBitSet getPressedBitset() const
	{
		if constexpr (I >= N - 1)
			return KeyBitSet{};
		else
			return bitsetOf(keys.at(I)) | getPressedBitset<I + 1>();
	}

	constexpr KeyBitSet getReleasedBitset() const
	{
		return bitsetOf(keys.at(keys.size() - 1));
	}
};

template <typename... K>
KeyCombination(K...) -> KeyCombination<sizeof...(K)>;

#ifdef SPATIAL_PLATFORM_OSX
constexpr auto gControlKey = Key::System;
#else
constexpr auto gControlKey = Key::LControl;
#endif

} // namespace spatial