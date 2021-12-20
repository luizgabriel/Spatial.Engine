#pragma once

#include "Resources.h"
namespace spatial::render
{

class TextureView
{
  public:
	explicit TextureView(filament::Engine& engine);

	TextureView(filament::Engine& engine, math::int2 size);

	void updateSize(math::int2 size);

	void setCamera(filament::Camera* camera);

	void setScene(filament::Scene* scene);

	[[nodiscard]] const auto& getColorTexture() const
	{
		return mRenderColorTexture;
	}

	[[nodiscard]] const auto& getView() const
	{
		return mView;
	}

  private:
	filament::Engine* mEngine;
	View mView;

	Texture mRenderColorTexture;
	Texture mRenderDepthTexture;
	RenderTarget mRenderTarget;
};

} // namespace spatial::render