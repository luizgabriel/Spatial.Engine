#pragma once

#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/View.h>
#include <spatial/render/EngineResource.h>
#include <spatial/desktop/Window.h>
#include <vector>

namespace spatial::render
{

class RenderPipeline
{
private:
	const desktop::Window* m_window;
	filament::Engine* m_engine;

	SwapChainPtr m_swapChain;
	RendererPtr m_renderer;
	ScenePtr m_scene;
	ViewPtr m_view;
	CameraPtr m_camera;

public:
	RenderPipeline(desktop::Window* window, filament::Engine *engine);

	void onRender();
	void onWindowResized();

	const RendererPtr &getRenderer() const
	{
		return m_renderer;
	}
};

} // namespace spatial::render