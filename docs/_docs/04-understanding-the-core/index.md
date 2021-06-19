---
title: Understanding the Core
layout: default
nav_order: 4
has_children: true
---

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

# The Event Queue

The application owns the main event queue. When we connect our subsystems to the application routine, we're able to be notified of any event. These events can be input ones, like mouse-movement, mouse-scroll, key-press, window-resize; or game loop events, like frame-start, frame-update, frame-end, draw-ui. This event queue pattern is a nice decoupling way of communicating between subsystems and some resource events.

# Game Loop Events

All these member functions are optional. The connect function uses static reflections to check the existence of these functions. So, they must match the below signature.

```cpp
struct MySubSystem
{
    // Called once on the application start
    void onStart();

    // Called once on the application finish
    void onFinish();

    // Called before every frame, before onFrameUpdate()
    void onFrameStart(float delta);

    // Called every frame
    void onFrameUpdate(float delta);

    // Called every frame, after onFrameUpdate(). 
    // Note there is no `delta` param.
    void onFrameEnd();

    // Used for drawing debug user interface
    void onDrawGui();
};

```

To instantiate and connect the subsystem
```cpp
auto app = spatial::Application{};
auto mySys = MySubSystem{};
app >> mySys;
```

# Window Events

There is a slightly different approach for the window events.

```cpp
#include <spatial/desktop/PlatformEvent.h>
using namespace spatial;

struct MySubSystem
{
    // Called every time the window is resized
    void onEvent(const WindowResizedEvent& event);

    // Called every time the mouse is moved
    void onEvent(const MouseMovedEvent& event);
}
```

You just need to define `onEvent(const T&)` member function which T is the Event type.
There's a more common approach for handling mouse and keyboard input which we'll cover in the next topic.
