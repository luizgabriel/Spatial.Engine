#pragma once

#include <math/vec2.h>
#include <spatial/common/Key.h>

namespace spatial
{

class UserInterfaceInput
{
  public:
	UserInterfaceInput();

	void setup();

	void setMousePosition(filament::math::float2 position);
	void setKey(Key key, KeyAction action);
	void setText(const std::string& text);
};

} // namespace spatial