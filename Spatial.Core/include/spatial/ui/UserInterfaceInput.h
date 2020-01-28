#pragma once

#include <math/vec2.h>
#include <spatial/common/Key.h>
#include <spatial/common/KeyAction.h>

namespace spatial::ui
{

class UserInterfaceInput
{
public:
    UserInterfaceInput();

    void setup();

    void setMousePosition(filament::math::float2 position);
    void setKey(common::Key key, common::KeyAction action);
    void setText(const std::string& text);
};

}