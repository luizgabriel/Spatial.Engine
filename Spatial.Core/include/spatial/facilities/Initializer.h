#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/core/Application.h>
#include <spatial/common/Configuration.h>
#include <spatial/render/RenderingSystem.h>

namespace spatial
{

Window createDefaultWindow(Application& app, const Configuration& config);

int start(const Configuration& config, const std::function<void(Application&, RenderingSystem&)>& action);

} // namespace spatial