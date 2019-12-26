#include <spatial/common/KeyAction.h>
#include <iostream>

namespace spatial::common
{

std::string_view nameOf(KeyAction action)
{
    switch (action)
    {
    case KeyAction::None:
        return "TYPED";
    case KeyAction::Pressed:
        return "PRESSED";
        break;
    case KeyAction::Released:
        return "RELEASED";
    }
}

}