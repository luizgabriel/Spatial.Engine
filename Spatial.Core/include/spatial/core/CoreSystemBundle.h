#pragma once
#include <spatial/core/Application.h>
#include <spatial/core/System.h>
#include <spatial/desktop/Window.h>
#include <spatial/input/InputSystem.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/ui/UserInterfaceSystem.h>
#include <toml++/toml.h>

namespace spatial
{
struct CoreSystemBundle
{
	Window window;
	System<InputSystem> input;
	System<RenderingSystem> rendering;
	System<UserInterfaceSystem> ui;

	explicit CoreSystemBundle(Application& app, const toml::table& config);

	auto& getRenderingSystem() { return rendering.get(); }
	auto& getInputSystem() { return input.get(); }
	auto& getUserInterfaceSystem() { return ui.get(); }
};
}
