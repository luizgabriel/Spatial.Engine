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
    void set(common::Key key, common::KeyAction action)
    {
        if (action == common::KeyAction::Pressed)
            setPressed(key);
        else if (action == common::KeyAction::Released)
            setReleased(key);
        else
            reset(key);
    }

    void setPressed(common::Key key)
    {
        auto flag = static_cast<size_t>(key);
        m_keyPressed.set(flag);
        m_keyReleased.reset(flag);
    }

    void setReleased(common::Key key)
    {
        auto flag = static_cast<size_t>(key);
        m_keyPressed.reset(flag);
        m_keyReleased.set(flag);
    }

    void reset(common::Key key)
    {
        auto flag = static_cast<size_t>(key);
        m_keyPressed.reset(flag);
        m_keyReleased.reset(flag);
    }

    void reset()
    {
        m_keyPressed.reset();
        m_keyReleased.reset();
    }

    void setMousePosition(filament::math::float2 position)
    {
        m_mousePosition = position;
    }

    bool isPressed(common::Key key) const
    {
        return m_keyPressed.test(static_cast<size_t>(key));
    }

    bool isReleased(common::Key key) const
    {
        return m_keyReleased.test(static_cast<size_t>(key));
    }

    filament::math::float2 getMousePosition() const
    {
        return m_mousePosition;
    }

};

} // namespace spatial::input