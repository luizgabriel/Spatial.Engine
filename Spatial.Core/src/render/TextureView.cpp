#include <filament/Viewport.h>
#include <spatial/render/Resources.h>
#include <spatial/render/TextureView.h>

namespace spatial::render
{

TextureView::TextureView(filament::Engine& engine)
	: mEngine{&engine},
	  mView{render::createView(engine)},
	  mRenderColorTexture{engine, nullptr},
	  mRenderDepthTexture{engine, nullptr},
	  mRenderTarget{engine, nullptr}
{
}

TextureView::TextureView(filament::Engine& engine, math::int2 size) : TextureView(engine)
{
	updateSize(size);
}

void TextureView::updateSize(math::int2 size)
{
	mView->setViewport({0, 0, static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y)});

	mRenderColorTexture =
		createTexture(*mEngine, size, filament::backend::TextureFormat::RGBA16F,
					  filament::Texture::Usage::COLOR_ATTACHMENT | filament::Texture::Usage::SAMPLEABLE);
	mRenderDepthTexture = createTexture(*mEngine, size, filament::backend::TextureFormat::DEPTH16,
										filament::Texture::Usage::DEPTH_ATTACHMENT);

	mRenderTarget = render::createRenderTarget(*mEngine, *mRenderColorTexture, *mRenderDepthTexture);

	mView->setRenderTarget(mRenderTarget.get());
}

void TextureView::setCamera(filament::Camera* camera)
{
	mView->setCamera(camera);
}

void TextureView::setScene(filament::Scene* scene)
{
	mView->setScene(scene);
}

} // namespace spatial::render