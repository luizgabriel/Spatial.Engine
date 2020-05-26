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
	std::reference_wrapper<Window> window;
	std::reference_wrapper<RenderingSystem> rendering;
	std::reference_wrapper<InputSystem> input;
	std::reference_wrapper<UserInterfaceSystem> ui;
};

Window createDefaultWindow(const Application& app, const Configuration& config);

int setup(const Configuration& config, std::function<int(Application&, SystemServices&)>&& action);

} // namespace spatial