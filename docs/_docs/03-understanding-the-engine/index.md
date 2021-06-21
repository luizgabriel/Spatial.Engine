---
title: Understanding the Engine
layout: default
nav_order: 3
has_children: true
has_toc: false
---

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

## The Engine Entry Point

Now we will understand how `Spatial.Editor` makes use of the engine's features.

If you open the file `Spatial.Editor\src\main.cpp`, you will see the starting point of the executable.
```cpp
int main(int argc, char* argv[])
{
	const auto args = argh::parser(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);
	const auto executablePath = fs::path{args[0]}.parent_path();

	auto config = SetupConfig{"Spatial Engine | Editor", 1280, 720};
	args({"-w", "--width"}) >> config.windowWidth;
	args({"-h", "--height"}) >> config.windowHeight;

	auto app = Application{};
	auto desktopContext = desktop::PlatformContext{};
	auto window = desktopContext.createWindow(config.windowWidth, config.windowHeight, config.windowTitle);
	auto rendering = RenderingSystem{fl::backend::Backend::OPENGL, window};
	auto input = desktop::InputSystem{window};
	auto ui = UserInterfaceSystem{rendering.getEngine(), window};
	ui.setMaterial(toShared(createMaterial(rendering.getEngine(), {ASSETS_UI_BLIT_FILAMAT, ASSETS_UI_BLIT_FILAMAT_SIZE})));
	ui.setFontTexture(toShared(imguiCreateTextureAtlas(rendering.getEngine(), {ASSETS_ROBOTO_MEDIUM_TTF, ASSETS_ROBOTO_MEDIUM_TTF_SIZE})));

	auto editor = editor::SceneEditorSystem{rendering.getEngine(), window, input.getState()};

	// Connect all Systems to the Application Main Loop
	app >> desktopContext >> input >> rendering >> ui >> editor;

	// Connect Desktop Events to All Systems
	desktopContext >> app >> input >> rendering >> ui >> editor;

	// Connect Gui Render to Editor
	ui >> editor;

	// Connect Rendering to Editor and UI
	rendering >> editor >> ui;

	return app.run();
}
```
See also [Spatial.Editor/src/main.cpp](https://github.com/luizgabriel/Spatial.Engine/blob/cc6096997b3546c87d4cc45d33a69b275f1dd828/Spatial.Editor/src/main.cpp#L29)

## Parsing Command Line Arguments
This part it's not related to the engine itself. The editor was design to be started from the command line, so the [Argh! Library](https://github.com/adishavit/argh) was used for that purpose.
```cpp
const auto args = argh::parser(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);
const auto executablePath = fs::path{args[0]}.parent_path();

auto config = SetupConfig{"Spatial Engine | Editor", 1280, 720};
args({"-w", "--width"}) >> config.windowWidth;
args({"-h", "--height"}) >> config.windowHeight;
```
The first argument it's always the executable path. We can use this information for reading asset files relative to the working directory.
It also accepts a width and height options.

## The Application

The `Application` its the subsystem which controls the [Game Loop](https://gameprogrammingpatterns.com/game-loop.html#take-a-little-nap) (we'll talk more about the Event System later). Without the application, all other subsystems are just loose parts with no behavior.

```cpp
auto app = Application{};
```

## The Desktop Platform

The engine editor it's a desktop application. To create desktop windows we need to create the context first. Later we may implement other type of contexts.

```cpp
auto desktopContext = desktop::PlatformContext{};
auto window = desktopContext.createWindow(config.windowWidth, config.windowHeight, config.windowTitle);
```

## The Rendering System
This is the subsystem which wraps the usage of the [Filament Library](https://github.com/google/filament). The rendering engine depends on the window to create the OpenGL Context, initialize buffers, etc.

```cpp
auto rendering = RenderingSystem{fl::backend::Backend::OPENGL, window};
```

You can also instantiate the rendering engine as:
```cpp
auto rendering = RenderingSystem{fl::backend::Backend::OPENGL, window.getNativeHandle()};
```
The `window.getNativeHandle()` method needs to return the native window handle (HWND on Windows, X11 Window for Unix and NSView for MacOS)


## The Input System
```cpp
auto input = desktop::InputSystem{window};
```

This line creates a system responsible for mantanning the `desktop::InputState` of the `desktop::PlatformContext`.
We need to call `input.getState()` to get the read input or mouse in the game loop. 

> PS.: There is no "Input::pressed(...)" facade or singleton here. Just clear dependencies throughout the code. 
> As a design mindset we tend to avoid the usage of Singletons because of its undesired effects it may cause on the application. 
>
> See also: [Retiring the Singleton Pattern: Concrete Suggestions for What to use Instead - Peter Muldoon](https://www.youtube.com/watch?v=K5c7uvWe_hw)


## The User Interface System
The `UserInterfaceSystem` its a wrapper implementation for the [ImGUI Library](https://github.com/ocornut/imgui) for the Filament's renderer.

```cpp
auto ui = UserInterfaceSystem{rendering.getEngine(), window};
ui.setMaterial(toShared(createMaterial(rendering.getEngine(), {ASSETS_UI_BLIT_FILAMAT, ASSETS_UI_BLIT_FILAMAT_SIZE})));
ui.setFontTexture(toShared(imguiCreateTextureAtlas(rendering.getEngine(), {ASSETS_ROBOTO_MEDIUM_TTF, ASSETS_ROBOTO_MEDIUM_TTF_SIZE})));
```

There are some resources which it depends one: the [ui_blit.mat](https://github.com/luizgabriel/Spatial.Engine/blob/cc6096997b3546c87d4cc45d33a69b275f1dd828/Spatial.Editor/assets/materials/ui_blit.mat#L23) and the Roboto Medium Font. As it's not good to be carring assets all around with the editor executable just to make the engine works, we compile and embed these resources with the code and access it through these generated `ASSETS_*` variables.

## The Editor System

Here where the editor logic lives and dies. You can create your own systems too and uses as your `Game` class, it's up to you.
```cpp
auto editor = editor::SceneEditorSystem{rendering.getEngine(), window, input.getState()};
```

Can you see that the editor depends on the _engine_, the instantiated _window_ and the _input state_? Clear dependencies make things more readable and I personally like it. It needs the engine to create renderables, light, scenes and etc. It needs the window class for calling `warpMouse` commands (this may change in the future). And also it needs to read the input state for performing some actions based on your input and mouse at runtime.

## Linking all together

Now we need to connect everything together through the event system. 
```cpp
// Connect all Systems to the Application Main Loop
app >> desktopContext >> input >> rendering >> ui >> editor;

// Connect Desktop Events to All Systems
desktopContext >> app >> input >> rendering >> ui >> editor;

// Connect Gui Render to Editor
ui >> editor;

// Connect Rendering to Editor and UI
rendering >> editor >> ui;
```

The `>>` operator it's just sugar syntax for:
```cpp
connect(app, desktopContext);
connect(app, input);
connect(app, rendering);
connect(app, ui);
connect(app, editor);
```
