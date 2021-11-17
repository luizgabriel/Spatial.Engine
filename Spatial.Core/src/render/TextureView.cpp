#include <filament/Viewport.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/TextureView.h>

namespace spatial::render
{

TextureView::TextureView(filament::Engine& engine, math::int2 size)
	:
	  mRenderColorTexture{
		  render::createTexture(engine, size, filament::backend::TextureFormat::RGBA16F,
								filament::Texture::Usage::COLOR_ATTACHMENT | filament::Texture::Usage::SAMPLEABLE)},
	  mRenderDepthTexture{render::createTexture(engine, size, filament::backend::TextureFormat::DEPTH16,
												filament::Texture::Usage::DEPTH_ATTACHMENT)},
	  mRenderTarget{render::createRenderTarget(engine, mRenderColorTexture.ref(), mRenderDepthTexture.ref())},
	  mView{render::createView(engine)}
{
	mView->setViewport({0, 0, static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y)});
	mView->setRenderTarget(mRenderTarget.get());
}

} // namespace spatial::render