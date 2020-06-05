#pragma once

#include <spatial/common/EventQueue.h>

#include <spatial/desktop/PlatformEvent.h>

#include <spatial/render/Engine.h>
#include <spatial/render/Resources.h>

#include <filament/Viewport.h>

#include <deque>
#include <spatial/desktop/Window.h>

namespace spatial
{
class RenderingSystem
{
private:
	Engine m_engine;
	SwapChain m_swapChain;
	Renderer m_renderer;

	Camera m_mainCamera;
	SharedView m_mainView;

	std::deque<std::weak_ptr<filament::View>> m_views;

	void setupViewport(const std::pair<int, int>& frameBufferSize);

public:
	explicit RenderingSystem(const Window& window);
	RenderingSystem(const Window& window, filament::backend::Backend backend);

	RenderingSystem(const RenderingSystem& other) = delete;
	RenderingSystem& operator=(const RenderingSystem& w) = delete;

	void onStart();

	void onEndFrame();

	void onEvent(const WindowResizedEvent& event);

	/**
	 * \brief Registers a view to the renderer
	 */
	void pushFrontView(std::weak_ptr<filament::View>&& view);

	/**
	 * \brief Deregisters a view to the renderer
	 */
	void popFrontView();

	/**
	 * \brief Registers a view to the renderer
	 */
	void pushBackView(std::weak_ptr<filament::View>&& view);

	/**
	 * \brief Pops a view to the renderer
	 */
	void popBackView();

	// region Getters
	[[nodiscard]] const auto& getEngine() const
	{
		return *m_engine.get();
	}

	[[nodiscard]] const filament::View& getMainView() const
	{
		return *m_mainView.get();
	}

	[[nodiscard]] const filament::Camera& getMainCamera() const
	{
		return *m_mainCamera.get();
	}

	auto& getEngine()
	{
		return *m_engine.get();
	}

	auto& getMainView()
	{
		return *m_mainView.get();
	}

	auto& getMainCamera()
	{
		return *m_mainCamera.get();
	}
	// endregion

private:
	void clearExpiredViews() noexcept;
};
} // namespace spatial
