#pragma once

#include <spatial/render/Engine.h>
#include <spatial/desktop/Window.h>
#include <vector>

namespace spatial::render
{

class RenderPipeline
{
private:
	desktop::Window* m_window;
	RenderEngine* m_engine;

	SwapChain m_swapChain;
	Renderer m_renderer;
	Scene m_scene;
	View m_view;
	Camera m_camera;

	std::uint32_t m_skippedFrames;

public:
	RenderPipeline(desktop::Window* window, RenderEngine* engine);

	void onRender();
	void onWindowResized();

	filament::View* getView() const
	{
		return m_view.get();
	}

	filament::Camera* getCamera() const
	{
		return m_camera.get();
	}

	std::uint32_t getSkippedFramesCount() const
	{
		return m_skippedFrames;
	}
};

} // namespace spatial::render