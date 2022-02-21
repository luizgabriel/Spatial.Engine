#pragma once

#include <spatial/render/RegistryRenderingSystem.h>

namespace spatial::render
{

template <typename Listener>
void connect(RegistryRenderingSystem& render, Listener& listener)
{
	render.getOnPublishRegistrySignal().connect<&Listener::onPublishRegistry>(listener);
}

template <typename Listener>
void disconnect(RegistryRenderingSystem& render, Listener& listener)
{
	render.getOnPublishRegistrySignal().disconnect<&Listener::onPublishRegistry>(listener);
}

template <typename Listener>
auto& operator>>(RegistryRenderingSystem& render, Listener& listener)
{
	connect(render, listener);
	return render;
}

} // namespace spatial::render