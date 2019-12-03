#include <spatial/EntryPoint.h>
#include <spatial/rendering/RenderingSubsystem.h>
#include <spatial/desktop/PlatformEvent.h>
#include <iostream>
#include <fmt/format.h>

namespace spatial
{

void printMousePosition(const desktop::MouseMoved &event)
{
    std::cout << fmt::format("MOUSE: ({0}, {1})\n", event.x, event.y);
}

void printWindowResized(const desktop::WindowResized &event)
{
    std::cout << fmt::format("RESIZE: ({0}, {1})\n", event.width, event.height);
}

int startEngine()
{
    RenderingSubsystem rendering;

    rendering.getWindowContext()->connect<desktop::MouseMoved, &printMousePosition>();
    rendering.getWindowContext()->connect<desktop::WindowResized, &printWindowResized>();

    while (!rendering.getWindow()->isClosed())
    {
        rendering.pollWindowEvents();

        rendering.onStartFrame();

        //

        rendering.onEndFrame();
    }

    return 0;
}

} // namespace spatial