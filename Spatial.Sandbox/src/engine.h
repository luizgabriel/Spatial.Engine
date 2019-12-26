#pragma once

#include <spatial/spatial.h>
#include <iostream>
#include <fmt/format.h>

spatial::core::Application& app();

inline spatial::render::RenderEngine& engine()
{
    return app().getRenderSys().getEngine();
}

inline const spatial::desktop::Window& window()
{
    return app().getRenderSys().getWindow();
}