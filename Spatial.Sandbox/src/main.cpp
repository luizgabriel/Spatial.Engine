#include <spatial/spatial.h>
#include <iostream>
#include <fmt/format.h>

using namespace spatial;

float debug = 0;

Application& app()
{
    static Application application;
    return application;
}

void onStart()
{
    auto& application = app();
    auto pipeline = application.getRenderPipeline();
    auto view = pipeline->getView();

    view->setClearColor({.0f, .86f, 0.98f, 1.0f});
}

void onUpdate(float delta)
{
    auto& application = app();
    auto pipeline = application.getRenderPipeline();

    if (debug > 2.0f) {
        std::cout << fmt::format("FPS: {}, Skipped Frames: {}\n", 1/delta, pipeline->getSkippedFramesCount());
        debug = 0;
    } else {
        debug += delta;
    }
}

int main(int arc, char* argv[])
{
    auto& application = app();
    application.onStartEvent.connect<&onStart>();
    application.onUpdateEvent.connect<&onUpdate>();

    return application.run();
}