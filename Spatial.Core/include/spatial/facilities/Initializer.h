#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/core/Application.h>
#include <spatial/common/Configuration.h>
#include <spatial/render/RenderingSystem.h>

namespace spatial
{

Window createDefaultWindow(const Application& app, const Configuration& config);

int setup(const Configuration& config, const std::function<int(Application&, RenderingSystem&)>& action);

} // namespace spatial