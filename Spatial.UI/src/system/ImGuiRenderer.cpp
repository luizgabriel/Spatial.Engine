#include <spatial/ui/system/ImGuiHelpers.h>
#include <spatial/ui/system/ImGuiRenderer.h>

#include <spatial/common/Math.h>

#include <spatial/graphics/Resources.h>
#include <spatial/graphics/SkyboxResources.h>

#include <filament/RenderableManager.h>
#include <filament/TextureSampler.h>
#include <filament/Viewport.h>

namespace fl = filament;

namespace spatial::ui
{

ImGuiRenderer::ImGuiRenderer(fl::Engine& engine)
	: mEngine{engine},
	  mView{graphics::createView(mEngine)},
	  mScene{graphics::createScene(mEngine)},
	  mCameraEntity{graphics::createEntity(mEngine)},
	  mCamera{graphics::createCamera(mEngine, mCameraEntity.get())},
	  mEntity{graphics::createEntity(mEngine)},
	  mSkybox{graphics::createSkybox(mEngine, fl::math::float4{.0f, .0f, .0f, .0f})},
	  mMaterial{},
	  mFontTexture{}
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

void ImGuiRenderer::setMaterial(const uint8_t* data, size_t size)
{
	mMaterial = toShared(graphics::createMaterial(mEngine, data, size));
}

void ImGuiRenderer::addFont(const uint8_t* data, size_t size)
{
	ImGui::SetCurrentContext(mImguiContext);
	ui::imguiAddFont(data, size, 15.0f);
}

ImGuiRenderer::~ImGuiRenderer()
{
	ImGui::DestroyContext(mImguiContext);
}

void ImGuiRenderer::setViewport(const math::vec2& windowSize, const math::vec2& frameBufferSize)
{
	const auto dpiScaleX = frameBufferSize.x / windowSize.x;
	const auto dpiScaleY = frameBufferSize.y / windowSize.y;

	mView->setViewport({0, 0, static_cast<uint32_t>(frameBufferSize.x), static_cast<uint32_t>(frameBufferSize.y)});

	const auto right = static_cast<double>(frameBufferSize.x) / static_cast<double>(dpiScaleX);
	const auto bottom = static_cast<double>(frameBufferSize.y) / static_cast<double>(dpiScaleY);
	mCamera.setOrthographicProjection(0.0, right, bottom, 0.0, 0.0, 1.0);

	const auto scaleX = windowSize.x > 0 ? dpiScaleX : 0;
	const auto scaleY = windowSize.y > 0 ? dpiScaleY : 0;

	ImGui::SetCurrentContext(mImguiContext);
	ui::imguiRefreshViewport(windowSize.x, windowSize.y, scaleX, scaleY);
}

void ImGuiRenderer::initNewFrame(float delta)
{
	ImGui::SetCurrentContext(mImguiContext);
	ui::imguiRefreshDeltaTime(delta);
	ImGui::NewFrame();
}

void ImGuiRenderer::drawFrame()
{
	ImGui::SetCurrentContext(mImguiContext);

	// Avoid rendering when minimized and scale coordinates for retina displays.
	if (ui::imguiIsMinimized())
		return;

	ImGui::Render();

	renderDrawData();
}

/**
 * TODO: Refactor to Entity-Component-System API Calls (Instead of direct render commands)
 */
void ImGuiRenderer::renderDrawData()
{
	auto imguiData = ImGui::GetDrawData();
	auto& rcm = mEngine.getRenderableManager();
	auto& io = ImGui::GetIO();
	auto fbSize = ui::imguiGetFrameSize();

	if (fbSize.x == 0 || fbSize.y == 0)
		return;

	imguiData->ScaleClipRects(io.DisplayFramebufferScale);

	// Ensure that we have enough vertex buffers and index buffers.
	createBuffers(static_cast<size_t>(imguiData->CmdListsCount));

	// Count how many primitives we'll need, then create a Renderable builder.
	size_t nPrims = 0;
	for (int cmdListIndex = 0; cmdListIndex < imguiData->CmdListsCount; cmdListIndex++)
	{
		const auto* commands = imguiData->CmdLists[cmdListIndex];
		nPrims += static_cast<size_t>(commands->CmdBuffer.size());
	}

	auto builder = fl::RenderableManager::Builder(nPrims);
	builder.boundingBox({{0, 0, 0}, {10000, 10000, 10000}}).culling(false);

	// Ensure that we have a material instance for each scissor rectangle.
	createMaterialInstances(nPrims);

	// Recreate the Renderable component and point it to the vertex buffers.
	rcm.destroy(mEntity.get());
	size_t bufferIndex = 0;
	size_t primIndex = 0;
	for (int cmdListIndex = 0; cmdListIndex < imguiData->CmdListsCount; cmdListIndex++)
	{
		const auto* commands = imguiData->CmdLists[cmdListIndex];
		populateVertexData(bufferIndex, commands->VtxBuffer);
		populateIndexData(bufferIndex, commands->IdxBuffer);

		for (const auto& command : commands->CmdBuffer)
		{
			if (command.UserCallback)
			{
				command.UserCallback(commands, &command);
			}
			else
			{
				auto& mi = mMaterialInstances[primIndex];
				mi.setScissor(static_cast<uint32_t>(command.ClipRect.x),
							  static_cast<uint32_t>(fbSize.y - command.ClipRect.w),
							  static_cast<uint32_t>(command.ClipRect.z - command.ClipRect.x),
							  static_cast<uint32_t>(command.ClipRect.w - command.ClipRect.y));

				if (command.TextureId)
				{
					auto albedo = std::static_pointer_cast<filament::Texture>(command.TextureId);
					mi.setParameter("albedo", std::move(albedo),
									fl::TextureSampler{fl::TextureSampler::MinFilter::LINEAR,
													   fl::TextureSampler::MagFilter::LINEAR});
				}
				else
				{
					mi.setParameter("albedo", mFontTexture,
									fl::TextureSampler{fl::TextureSampler::MinFilter::NEAREST,
													   fl::TextureSampler::MagFilter::NEAREST});
				}

				builder
					.geometry(primIndex, fl::RenderableManager::PrimitiveType::TRIANGLES,
							  mVertexBuffers[bufferIndex].get(), mIndexBuffers[bufferIndex].get(), command.IdxOffset,
							  command.ElemCount)
					.blendOrder(primIndex, static_cast<uint16_t>(primIndex))
					.material(primIndex, mi.getInstance());

				primIndex++;
			}
		}

		bufferIndex++;
	}

	if (imguiData->CmdListsCount > 0)
		builder.build(mEngine, mEntity.get());
}

void ImGuiRenderer::createBuffers(size_t numRequiredBuffers)
{
	if (numRequiredBuffers > mVertexBuffers.size())
	{
		const size_t previousSize = mVertexBuffers.size();
		mVertexBuffers.reserve(numRequiredBuffers);

		for (size_t i = previousSize; i < numRequiredBuffers; i++)
		{
			// Pick a reasonable starting capacity; it will grow if needed.
			mVertexBuffers.emplace_back(ui::imguiCreateVertexBuffer(mEngine, 1000));
		}
	}

	if (numRequiredBuffers > mIndexBuffers.size())
	{
		const size_t previousSize = mIndexBuffers.size();
		mIndexBuffers.reserve(numRequiredBuffers);

		for (size_t i = previousSize; i < numRequiredBuffers; i++)
		{
			// Pick a reasonable starting capacity; it will grow if needed.
			mIndexBuffers.emplace_back(ui::imguiCreateIndexBuffer(mEngine, 5000));
		}
	}
}

void ImGuiRenderer::createMaterialInstances(size_t numRequiredInstances)
{
	const size_t previousSize = mMaterialInstances.size();
	if (numRequiredInstances > mMaterialInstances.size())
	{
		mMaterialInstances.reserve(numRequiredInstances);
		for (size_t i = previousSize; i < numRequiredInstances; i++)
			mMaterialInstances.emplace_back(graphics::createMaterialInstance(mEngine, mMaterial));
	}
}

void ImGuiRenderer::populateVertexData(size_t bufferIndex, const ImVector<ImDrawVert>& vb)
{
	// Create a new vertex buffer if the size isn't large enough, then copy the ImGui data into
	// a staging area since Filament's render thread might consume the data at any time.
	const size_t capacityVertCount = mVertexBuffers[bufferIndex]->getVertexCount();
	if (static_cast<size_t>(vb.Size) > capacityVertCount)
		mVertexBuffers[bufferIndex] = ui::imguiCreateVertexBuffer(mEngine, static_cast<uint32_t>(vb.Size));

	auto vbDescriptor = ui::imguiCreateDescriptor<ImDrawVert>(vb);
	mVertexBuffers[bufferIndex]->setBufferAt(mEngine, 0, std::move(vbDescriptor));
}

void ImGuiRenderer::populateIndexData(size_t bufferIndex, const ImVector<ImDrawIdx>& ib)
{
	// Create a new index buffer if the size isn't large enough, then copy the ImGui data into
	// a staging area since Filament's render thread might consume the data at any time.
	const size_t capacityIndexCount = mIndexBuffers[bufferIndex]->getIndexCount();
	if (static_cast<size_t>(ib.Size) > capacityIndexCount)
		mIndexBuffers[bufferIndex] = ui::imguiCreateIndexBuffer(mEngine, static_cast<uint32_t>(ib.Size));

	auto ibDescriptor = ui::imguiCreateDescriptor<ImDrawIdx>(ib);
	mIndexBuffers[bufferIndex]->setBuffer(mEngine, std::move(ibDescriptor));
}

void ImGuiRenderer::createFontTextureAtlas()
{
	assert(mMaterial);
	mFontTexture = toShared(ui::imguiCreateTextureAtlas(mEngine));
	mMaterial->setDefaultParameter("albedo", mFontTexture.get(),
								   {fl::TextureSampler::MinFilter::LINEAR, fl::TextureSampler::MagFilter::LINEAR});
}

} // namespace spatial::ui
