#pragma once

#include <spatial/render/RenderingSystem.h>

namespace spatial::render
{

template <typename Listener>
void connect(RenderingSystem& render, Listener& listener)
{
	render.getOnRenderSignal().connect<&Listener::onRender>(listener);
}

template <typename Listener>
void disconnect(RenderingSystem& render, Listener& listener)
{
	render.getOnRenderSignal().disconnect<&Listener::onRender>(listener);
}

template <typename Listener>
auto& operator>>(RenderingSystem& render, Listener& listener)
{
	connect(render, listener);
	return render;
}

} // namespace spatial