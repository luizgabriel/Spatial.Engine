#pragma once

#include "Resources.h"
namespace spatial::render
{

class TextureView
{
  public:
	TextureView(filament::Engine& engine, math::int2 size);

	const auto& getColorTexture() const
	{
		return mRenderColorTexture;
	}

	auto& getColorTexture()
	{
		return mRenderColorTexture;
	}

	const auto& getView() const
	{
		return mView;
	}

	auto& getView()
	{
		return mView;
	}

  private:
	Texture mRenderColorTexture;
	Texture mRenderDepthTexture;
	RenderTarget mRenderTarget;
	View mView;
};

}