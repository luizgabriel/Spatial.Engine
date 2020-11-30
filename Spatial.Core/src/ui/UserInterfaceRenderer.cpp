#include <spatial/ui/ImGuiHelpers.h>
#include <spatial/ui/UserInterfaceRenderer.h>

#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/SkyboxResources.h>

#include <filament/IndexBuffer.h>
#include <filament/RenderableManager.h>
#include <filament/TextureSampler.h>
#include <filament/VertexBuffer.h>
#include <filament/Viewport.h>
#include <unordered_map>

namespace fl = filament;

namespace spatial
{

UserInterfaceRenderer::UserInterfaceRenderer(fl::Engine& engine)
	: mEngine{engine},
	  mView{toShared(createView(mEngine))},
	  mScene{createScene(mEngine)},
	  mCameraEntity{createEntity(mEngine)},
	  mCamera{createCamera(mEngine, mCameraEntity.get())},
	  mMaterial{mEngine},
	  mEntity{createEntity(mEngine)},
	  mSkybox{createSkybox(mEngine, fl::math::float4{.0f, .0f, .0f, .0f})},
	  mFontTexture{mEngine}
{
	mView->setCamera(mCamera.getInstance());
	mView->setScene(mScene.get());
	mScene->setSkybox(mSkybox.get());
	mScene->addEntity(mEntity.get());

	mView->setPostProcessingEnabled(false);
	mView->setBlendMode(filament::View::BlendMode::TRANSLUCENT);
	mView->setShadowingEnabled(false);

	mImguiContext = ImGui::CreateContext();
}

void UserInterfaceRenderer::setMaterial(const std::vector<char>& materialData)
{
	mMaterial = createMaterial(mEngine, materialData);
}

void UserInterfaceRenderer::setFont(const std::vector<char>& fontData)
{
	mFontTexture = imguiCreateTextureAtlas(mEngine, fontData);

	mMaterial->setDefaultParameter("albedo", mFontTexture.get(),
								   {fl::TextureSampler::MinFilter::LINEAR, fl::TextureSampler::MagFilter::LINEAR});
}

void UserInterfaceRenderer::setupEngineTheme()
{
	auto& io = ImGui::GetIO();
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_DpiEnableScaleFonts | ImGuiConfigFlags_IsSRGB;

	auto& style = ImGui::GetStyle();
	ImGui::StyleColorsDark(&style);
}

UserInterfaceRenderer::~UserInterfaceRenderer()
{
	ImGui::DestroyContext(mImguiContext);
}

void UserInterfaceRenderer::setViewport(const std::pair<int, int>& windowSize,
										const std::pair<int, int>& frameBufferSize)
{
	const auto [w, h] = windowSize;
	const auto [fw, fh] = frameBufferSize;

	const auto dpiScaleX = static_cast<float>(fw) / w;
	const auto dpiScaleY = static_cast<float>(fh) / h;

	mView->setViewport({0, 0, static_cast<uint32_t>(fw), static_cast<uint32_t>(fh)});
	mCamera.setOrthographicProjection(0.0, fw / dpiScaleX, fh / dpiScaleY, 0.0, 0.0, 1.0);

	const auto scaleX = w > 0 ? static_cast<float>(fw) / w : 0;
	const auto scaleY = h > 0 ? static_cast<float>(fh) / h : 0;
	imguiRefreshViewport(w, h, scaleX, scaleY);
}

void UserInterfaceRenderer::beforeRender(float delta) const
{
	ImGui::SetCurrentContext(mImguiContext);
	imguiRefreshDeltaTime(delta);
	ImGui::NewFrame();
}

void UserInterfaceRenderer::dispatchCommands()
{
	// Avoid rendering when minimized and scale coordinates for retina displays.
	if (imguiIsMinimized())
		return;

	ImGui::Render();

	renderDrawData();
}

void UserInterfaceRenderer::renderDrawData()
{
	auto imguiData = ImGui::GetDrawData();
	auto& rcm = mEngine.getRenderableManager();
	auto& io = ImGui::GetIO();
	auto [fbWidth, fbHeight] = imguiGetFrameSize();

	if (fbWidth == 0 || fbHeight == 0)
		return;

	imguiData->ScaleClipRects(io.DisplayFramebufferScale);

	// Ensure that we have enough vertex buffers and index buffers.
	createBuffers(imguiData->CmdListsCount);

	// Count how many primitives we'll need, then create a Renderable builder.
	size_t nPrims = 0;
	for (int cmdListIndex = 0; cmdListIndex < imguiData->CmdListsCount; cmdListIndex++) {
		const ImDrawList* cmds = imguiData->CmdLists[cmdListIndex];
		nPrims += cmds->CmdBuffer.size();
	}

	auto rBuilder = fl::RenderableManager::Builder(nPrims);
	rBuilder.boundingBox({{0, 0, 0}, {10000, 10000, 10000}}).culling(false);

	// Ensure that we have a material instance for each scissor rectangle.
	createMaterialInstances(nPrims);

	// Recreate the Renderable component and point it to the vertex buffers.
	rcm.destroy(mEntity.get());
	int bufferIndex = 0;
	int primIndex = 0;
	for (int cmdListIndex = 0; cmdListIndex < imguiData->CmdListsCount; cmdListIndex++)
	{
		const ImDrawList* cmds = imguiData->CmdLists[cmdListIndex];
		size_t indexOffset = 0;
		populateVertexData(bufferIndex, cmds->VtxBuffer, cmds->IdxBuffer);

		for (const auto& pcmd : cmds->CmdBuffer)
		{
			if (pcmd.UserCallback)
			{
				pcmd.UserCallback(cmds, &pcmd);
			}
			else
			{
				auto& mi = mMaterialInstances[primIndex];
				mi->setScissor( pcmd.ClipRect.x, fbHeight - pcmd.ClipRect.w,
											  (uint16_t) (pcmd.ClipRect.z - pcmd.ClipRect.x),
											  (uint16_t) (pcmd.ClipRect.w - pcmd.ClipRect.y));

				if (pcmd.TextureId) {
					mi->setParameter("is_texture", true);
					mi->setParameter("albedo", reinterpret_cast<const fl::Texture*>(pcmd.TextureId), fl::TextureSampler{fl::TextureSampler::MinFilter::LINEAR, fl::TextureSampler::MagFilter::LINEAR});
				} else {
					mi->setParameter("albedo", mFontTexture.get(), fl::TextureSampler{fl::TextureSampler::MinFilter::LINEAR, fl::TextureSampler::MagFilter::LINEAR});
					mi->setParameter("is_texture", false);
				}

				rBuilder
					.geometry(primIndex, fl::RenderableManager::PrimitiveType::TRIANGLES,
							  mVertexBuffers[bufferIndex].get(), mIndexBuffers[bufferIndex].get(),
							  indexOffset, pcmd.ElemCount)
					.blendOrder(primIndex, primIndex)
					.material(primIndex, mi.get());

				primIndex++;
			}
			indexOffset += pcmd.ElemCount;
		}

		bufferIndex++;
	}

	if (imguiData->CmdListsCount > 0)
	{
		rBuilder.build(mEngine, mEntity.get());
	}
}

void UserInterfaceRenderer::createBuffers(size_t numRequiredBuffers)
{
	if (numRequiredBuffers > mVertexBuffers.size())
	{
		const size_t previousSize = mVertexBuffers.size();
		mVertexBuffers.reserve(numRequiredBuffers);

		for (size_t i = previousSize; i < numRequiredBuffers; i++)
		{
			// Pick a reasonable starting capacity; it will grow if needed.
			mVertexBuffers.emplace_back(imguiCreateVertexBuffer(mEngine, 1000));
		}
	}


	if (numRequiredBuffers > mIndexBuffers.size())
	{
		const size_t previousSize = mIndexBuffers.size();
		mIndexBuffers.reserve(numRequiredBuffers);

		for (size_t i = previousSize; i < numRequiredBuffers; i++)
		{
			// Pick a reasonable starting capacity; it will grow if needed.
			mIndexBuffers.emplace_back(imguiCreateIndexBuffer(mEngine, 5000));
		}
	}
}

void UserInterfaceRenderer::createMaterialInstances(size_t numRequiredInstances)
{
	const size_t previousSize = mMaterialInstances.size();
	if (numRequiredInstances > mMaterialInstances.size())
	{
		mMaterialInstances.reserve(numRequiredInstances);
		for (size_t i = previousSize; i < numRequiredInstances; i++)
		{
			mMaterialInstances.emplace_back(mEngine, mMaterial->createInstance());
		}
	}
}

void UserInterfaceRenderer::populateVertexData(size_t bufferIndex, const ImVector<ImDrawVert>& vb,
											   const ImVector<ImDrawIdx>& ib)
{
	// Create a new vertex buffer if the size isn't large enough, then copy the ImGui data into
	// a staging area since Filament's render thread might consume the data at any time.
	{
		const size_t capacityVertCount = mVertexBuffers[bufferIndex]->getVertexCount();
		if (static_cast<size_t>(vb.Size) > capacityVertCount)
			mVertexBuffers[bufferIndex] = imguiCreateVertexBuffer(mEngine, vb.Size);

		auto vbDescriptor = imguiCreateDescriptor<ImDrawVert>(vb);
		mVertexBuffers[bufferIndex]->setBufferAt(mEngine, 0, std::move(vbDescriptor));
	}

	// Create a new index buffer if the size isn't large enough, then copy the ImGui data into
	// a staging area since Filament's render thread might consume the data at any time.
	{
		const size_t capacityIndexCount = mIndexBuffers[bufferIndex]->getIndexCount();
		if (static_cast<size_t>(ib.Size) > capacityIndexCount)
			mIndexBuffers[bufferIndex] = imguiCreateIndexBuffer(mEngine, ib.Size);

		auto ibDescriptor = imguiCreateDescriptor<ImDrawIdx>(ib);
		mIndexBuffers[bufferIndex]->setBuffer(mEngine, std::move(ibDescriptor));
	}
}

} // namespace spatial
