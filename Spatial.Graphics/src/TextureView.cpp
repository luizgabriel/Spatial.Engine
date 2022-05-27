#include <filament/Viewport.h>
#include <spatial/graphics/Resources.h>
#include <spatial/graphics/TextureResources.h>
#include <spatial/graphics/TextureView.h>

namespace spatial::graphics
{

TextureView::TextureView(filament::Engine& engine)
	: mEngine{&engine},
	  mView{graphics::createView(engine)},
	  mRenderColorTexture{engine, nullptr},
	  mRenderDepthTexture{engine, nullptr},
	  mRenderTarget{engine, nullptr}
{
}

TextureView::TextureView(filament::Engine& engine, math::uvec2 size) : TextureView(engine)
{
	updateSize(size);
}

void TextureView::updateSize(math::uvec2 size)
{
	mView->setViewport({0, 0, size.x, size.y});

	mRenderColorTexture =
		createTexture(*mEngine, size, filament::backend::TextureFormat::RGBA16F,
					  filament::Texture::Usage::COLOR_ATTACHMENT | filament::Texture::Usage::SAMPLEABLE);
	mRenderDepthTexture = createTexture(*mEngine, size, filament::backend::TextureFormat::DEPTH16,
										filament::Texture::Usage::DEPTH_ATTACHMENT);

	mRenderTarget = createRenderTarget(*mEngine, *mRenderColorTexture, *mRenderDepthTexture);

	mView->setRenderTarget(mRenderTarget.get());
}

void TextureView::setCamera(const SharedCamera& camera)
{
	mCamera = camera;
	mView->setCamera(mCamera->getInstance());
}

void TextureView::setScene(const SharedScene& scene)
{
	mScene = scene;
	mView->setScene(mScene.get());
}

} // namespace spatial::graphics