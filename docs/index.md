---
layout: default
---

<h1 class="text-center">Welcome to the Spatial Engine!</h1>

<div class="d-flex flex-column align-center">
    <img src="{{site.baseurl}}/assets/images/preview.png" alt="Engine Screenshot Preview"/>
    <p class="text-center">
        Spatial is a cross-platform C++ game engine created on top off Google's filament rendering engine.<br/>
        This projects uses C++17 and modern cmake features.
    </p>
    <div class="d-flex">
        <a href="{{site.baseurl}}/docs/01-building-editor" class="btn btn-blue">Building the Editor</a>
        <a href="https://github.com/luizgabriel/Spatial.Engine" class="btn ml-2">View on Github</a>
    </div>
</div>

## Why do we need another game engine?

Maybe you're asking yourself: "Wait a minute. Why do we need another game engine?" - and you're right.
There are plenty of well known game engines like Unity, Unreal Engine, some open-source like Panda 3D, Xenko and
Lumberyard Engine, etc.
So, if you REALLY want to make a game, go straight away with those engines.
But, if you need a simple visualization tool with game engine like tools and functions, I think that the Spatial Engine
is a nice solution to consider.

Programming a game engine is hard and the necessary skill are completely different from creating a game.
The Spatial Engine is still pretty simple and also really far from complete, but it already does some neat things.

## The Project Structure

Now, let's dive into the usage details of the Spatial Engine.

The project is organized by grouping some bigger modules:

```
Spatial.Engine
│   README.md
└── Spatial.Core (Contains everything the engine offers for you)
└── Spatial.UI (The user interface library)
└── Spatial.Editor (The engine scene editor built using with all the Core tools)
└── Spatial.Res (A C++ code generator for embedding resource files inside the Spatial.Editor binary)
└── Spatial.Game (An example game with some assets for you to play with the engine editor)
```

You can take a look at the `Spatial.Core` folder to understand how the darker details have been implemented.

## First time here, buddy?

So, the first step to get used to this engine is to build it from source.
We'll need to install the correct compiler, the package manager, run some installation scripts, wait for all required
dependencies to build, and finally you'll be able to see something on your screen.
You might take some time in this step if you're not used to all complexities of a C++ project.

When you get used with the engine, we can bootstrap a simple game project using the engine's core library.

## Let us known if you're having any trouble

As a reminder, this engine is improving constantly and changes may happen which could deprecate this tutorial steps.
So, if you have any problems, feel free to leave an issue on GitHub.

[Create a Github Issue](https://github.com/luizgabriel/Spatial.Engine/issues/new){: .btn }

## Now, let's build the Editor

<a href="{{site.baseurl}}/docs/01-building-editor" class="btn btn-blue">Go To: "Building the Editor"</a>