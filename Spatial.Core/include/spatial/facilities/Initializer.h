#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/core/Application.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/input/InputSystem.h>

#include <functional>

namespace spatial
{

struct SystemServices
{
	Window& window;
	RenderingSystem& rendering;
	InputSystem& input;
};

struct SetupConfig
{
	std::string_view windowTitle;
	int windowWidth;
    int windowHeight;
};

int setup(const SetupConfig& config, std::function<int(Application&, SystemServices&)>&& action);

} // namespace spatial