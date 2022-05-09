#pragma once

#include <spatial/render/Resources.h>

namespace spatial::render
{

class TextureView
{
  public:
	explicit TextureView(filament::Engine& engine);

	TextureView(filament::Engine& engine, math::int2 size);

	void updateSize(math::int2 size);

	void setCamera(const SharedCamera& camera);

	void setScene(const SharedScene& scene);

	[[nodiscard]] const auto& getColorTexture() const
	{
		return mRenderColorTexture;
	}

	[[nodiscard]] const auto& getView() const
	{
		return mView;
	}

	auto& getScene()
	{
		return mScene;
	}

	[[nodiscard]] const auto& getScene() const
	{
		return mScene;
	}

	[[nodiscard]] const auto& getCamera() const
	{
		return mScene;
	}

	[[nodiscard]] auto& getCamera()
	{
		return mScene;
	}

  private:
	filament::Engine* mEngine;
	SharedCamera mCamera;
	SharedScene mScene;

	View mView;
	Texture mRenderColorTexture;
	Texture mRenderDepthTexture;
	RenderTarget mRenderTarget;
};

} // namespace spatial::render