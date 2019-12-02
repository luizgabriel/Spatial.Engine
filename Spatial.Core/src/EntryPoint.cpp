#include <spatial/EntryPoint.h>
#include <spatial/rendering/RenderingSubsystem.h>

namespace spatial {

int startEngine()
{
    RenderingSubsystem rendering;

    while (!rendering.getWindow().isClosed()) {
        rendering.pollWindowEvents();

        rendering.onStartFrame();

        rendering.onUpdateFrame();

        rendering.onEndFrame();
    }

    return 0;
}

}