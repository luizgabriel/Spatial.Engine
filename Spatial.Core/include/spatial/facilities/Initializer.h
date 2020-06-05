#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/core/Application.h>
#include <spatial/common/Configuration.h>
#include <spatial/render/RenderingSystem.h>

#include <functional>
#include <spatial/input/InputSystem.h>
#include <spatial/ui/UserInterfaceSystem.h>

namespace spatial
{

struct SystemServices
{
	Window& window;
	RenderingSystem& rendering;
	InputSystem& input;
	UserInterfaceSystem& ui;
};

Window createDefaultWindow(const Application& app, const Configuration& config);

int setup(const Configuration& config, std::function<int(Application&, SystemServices&)>&& action);

} // namespace spatial