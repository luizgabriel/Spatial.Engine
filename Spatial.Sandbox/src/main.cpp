#include <spatial/spatial.h>
#include <iostream>
#include <fmt/format.h>

using namespace spatial;

void onUpdate(float delta)
{
    std::cout << fmt::format("FPS: {}\n", 1/delta);
}

int main(int arc, char* argv[])
{
    Application app;
    app.onUpdateEvent.connect<&onUpdate>();

    return app.run();
}