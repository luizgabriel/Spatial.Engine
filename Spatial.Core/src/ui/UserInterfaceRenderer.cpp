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
	  mView{createView(mEngine)},
	  mScene{createScene(mEngine)},
	  mCameraEntity{createEntity(mEngine)},
	  mCamera{createCamera(mEngine, mCameraEntity.get())},
	  mEntity{createEntity(mEngine)},
	  mSkybox{createSkybox(mEngine, fl::math::float4{.0f, .0f, .0f, .0f})},
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

void UserInterfaceRenderer::setMaterial(const SharedMaterial& material)
{
	mMaterial = material;
}

void UserInterfaceRenderer::setFontTexture(const SharedTexture& fontTextureAtlas)
{
	mFontTexture = fontTextureAtlas;
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
	auto& colors = style.Colors;
	colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_ChildBg]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_WindowBg]               = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PopupBg]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_Border]                 = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_FrameBg]                = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
	colors[ImGuiCol_TitleBg]                = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_CheckMark]              = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
	colors[ImGuiCol_SliderGrab]             = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
	colors[ImGuiCol_Button]                 = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
	colors[ImGuiCol_ButtonHovered]          = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
	colors[ImGuiCol_ButtonActive]           = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_Header]                 = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	colors[ImGuiCol_HeaderActive]           = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
	colors[ImGuiCol_Separator]              = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
	colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
	colors[ImGuiCol_SeparatorActive]        = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
	colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
	colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

	style.PopupRounding = 3;

	style.WindowPadding = ImVec2(4, 4);
	style.FramePadding  = ImVec2(6, 4);
	style.ItemSpacing   = ImVec2(6, 2);

	style.ScrollbarSize = 18;

	style.WindowBorderSize = 1;
	style.ChildBorderSize  = 1;
	style.PopupBorderSize  = 1;
	style.FrameBorderSize  = 1;

	style.WindowRounding    = 3;
	style.ChildRounding     = 3;
	style.FrameRounding     = 3;
	style.ScrollbarRounding = 2;
	style.GrabRounding      = 3;
	style.TabBorderSize = 1;
	style.TabRounding   = 3;

	colors[ImGuiCol_DockingEmptyBg]     = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_Tab]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabHovered]         = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_TabActive]          = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_TabUnfocused]       = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_DockingPreview]     = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);
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
	mCamera.setProjection(OrthographicProjection{0.0, fw / dpiScaleX, fh / dpiScaleY, 0.0, 0.0, 1.0});

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
