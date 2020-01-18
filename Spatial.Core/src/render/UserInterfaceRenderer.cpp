#include <spatial/render/UserInterfaceRenderer.h>
#include <spatial/render/ImGuiHelpers.h>
#include <spatial/render/ResourceLoaders.h>

#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/RenderableManager.h>

#include <filesystem>

using namespace spatial::common;
using namespace spatial::desktop;
namespace fs = std::filesystem;
namespace fl = filament;

namespace spatial::render
{

UserInterfaceRenderer::UserInterfaceRenderer(fl::Engine *engine)
	: m_engine{engine},
	  m_view{createView(m_engine)},
	  m_scene{createScene(m_engine)},
	  m_camera{createCamera(m_engine)},
	  m_material{createMaterial(m_engine, "materials/ui_blit.filamat")},
	  m_entity{createEntity(m_engine)},
	  m_texture{m_engine}
{
	m_view->setCamera(m_camera.get());
	m_view->setScene(m_scene.get());
	m_scene->addEntity(m_entity.get());

	m_view->setClearTargets(false, false, false);
	m_view->setRenderTarget(fl::View::TargetBufferFlags::DEPTH_AND_STENCIL);
	m_view->setPostProcessingEnabled(false);
	m_view->setShadowsEnabled(false);

	ImGui::CreateContext();
}

void UserInterfaceRenderer::setup(const fs::path& fontPath)
{
	m_texture = imguiCreateTextureAtlas(m_engine, fontPath);

	m_material->setDefaultParameter(
		"albedo", m_texture.get(),
		{fl::TextureSampler::MinFilter::LINEAR, fl::TextureSampler::MagFilter::LINEAR});

	auto &io = ImGui::GetIO();
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	auto &style = ImGui::GetStyle();

	ImGui::StyleColorsDark();
	auto colors = style.Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
}

UserInterfaceRenderer::~UserInterfaceRenderer()
{
	ImGui::DestroyContext();
}

void UserInterfaceRenderer::setViewport(int width, int height, float dpiX, float dpiY)
{
	m_view->setViewport({0, 0, static_cast<uint32_t>(width), static_cast<uint32_t>(height)});
	m_camera->setProjection(
		fl::Camera::Projection::ORTHO,
		0.0, width / dpiX, height / dpiY,
		0.0, 0.0, 1.0);

	imguiRefreshViewport(width, height, dpiX, dpiY);
}

void UserInterfaceRenderer::beforeRender(float delta)
{
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
	auto &rcm = m_engine->getRenderableManager();
	auto &io = ImGui::GetIO();
	auto [fbwidth, fbheight] = imguiGetFrameSize();

	imguiData->ScaleClipRects(io.DisplayFramebufferScale);

	// Ensure that we have enough vertex buffers and index buffers.
	createBuffers(imguiData->CmdListsCount);

	// Count how many primitives we'll need, then create a Renderable builder.
	// Also count how many unique scissor rectangles are required.
	size_t nPrims = 0;
	std::unordered_map<uint64_t, fl::MaterialInstance *> scissorRects;
	for (int cmdListIndex = 0; cmdListIndex < imguiData->CmdListsCount; cmdListIndex++)
	{
		const ImDrawList *cmds = imguiData->CmdLists[cmdListIndex];
		nPrims += cmds->CmdBuffer.size();
		for (const auto &pcmd : cmds->CmdBuffer)
		{
			scissorRects[imguiMakeScissorKey(fbheight, pcmd.ClipRect)] = nullptr;
		}
	}
	auto rbuilder = fl::RenderableManager::Builder(nPrims);
	rbuilder.boundingBox({{0, 0, 0}, {10000, 10000, 10000}}).culling(false);

	// Ensure that we have a material instance for each scissor rectangle.
	createMaterialInstances(scissorRects.size());

	// Push each unique scissor rectangle to a MaterialInstance.
	size_t matIndex = 0;
	for (auto &pair : scissorRects)
	{
		pair.second = m_materialInstances[matIndex++].get();
		uint32_t left = (pair.first >> 0ull) & 0xffffull;
		uint32_t bottom = (pair.first >> 16ull) & 0xffffull;
		uint32_t width = (pair.first >> 32ull) & 0xffffull;
		uint32_t height = (pair.first >> 48ull) & 0xffffull;
		pair.second->setScissor(left, bottom, width, height);
	}

	// Recreate the Renderable component and point it to the vertex buffers.
	rcm.destroy(m_entity.get());
	int bufferIndex = 0;
	int primIndex = 0;
	for (int cmdListIndex = 0; cmdListIndex < imguiData->CmdListsCount; cmdListIndex++)
	{
		const ImDrawList *cmds = imguiData->CmdLists[cmdListIndex];
		size_t indexOffset = 0;
		populateVertexData(bufferIndex, cmds->VtxBuffer, cmds->IdxBuffer);

		for (const auto &pcmd : cmds->CmdBuffer)
		{
			if (pcmd.UserCallback)
			{
				pcmd.UserCallback(cmds, &pcmd);
			}
			else
			{
				uint64_t skey = imguiMakeScissorKey(fbheight, pcmd.ClipRect);
				auto miter = scissorRects.find(skey);
				assert(miter != scissorRects.end());
				rbuilder.geometry(primIndex, fl::RenderableManager::PrimitiveType::TRIANGLES,
								  m_vertexBuffers[bufferIndex].get(), m_indexBuffers[bufferIndex].get(),
								  indexOffset, pcmd.ElemCount)
					.blendOrder(primIndex, primIndex)
					.material(primIndex, miter->second);
				primIndex++;
			}
			indexOffset += pcmd.ElemCount;
		}

		bufferIndex++;
	}

	if (imguiData->CmdListsCount > 0)
	{
		rbuilder.build(*m_engine, m_entity.get());
	}
}

void UserInterfaceRenderer::createBuffers(size_t numRequiredBuffers)
{
	if (numRequiredBuffers > m_vertexBuffers.size())
	{
		size_t previousSize = m_vertexBuffers.size();
		m_vertexBuffers.resize(numRequiredBuffers, nullptr);
		for (size_t i = previousSize; i < m_vertexBuffers.size(); i++)
		{
			// Pick a reasonable starting capacity; it will grow if needed.
			m_vertexBuffers[i] = createSharedResource(m_engine, imguiCreateVertexBuffer(m_engine, 1000));
		}
	}

	if (numRequiredBuffers > m_indexBuffers.size())
	{
		size_t previousSize = m_indexBuffers.size();
		m_indexBuffers.resize(numRequiredBuffers, nullptr);
		for (size_t i = previousSize; i < m_indexBuffers.size(); i++)
		{
			// Pick a reasonable starting capacity; it will grow if needed.
			m_indexBuffers[i] = createSharedResource(m_engine, imguiCreateIndexBuffer(m_engine, 5000));
		}
	}
}

void UserInterfaceRenderer::createMaterialInstances(size_t numRequiredInstances)
{
	size_t previousSize = m_materialInstances.size();
	if (numRequiredInstances > m_materialInstances.size())
	{
		m_materialInstances.resize(numRequiredInstances);
		for (size_t i = previousSize; i < m_materialInstances.size(); i++)
		{
			m_materialInstances[i] = createSharedResource(m_engine, m_material->createInstance());
		}
	}
}

void UserInterfaceRenderer::populateVertexData(size_t bufferIndex, const ImVector<ImDrawVert> &vb, const ImVector<ImDrawIdx> &ib)
{
	// Create a new vertex buffer if the size isn't large enough, then copy the ImGui data into
	// a staging area since Filament's render thread might consume the data at any time.
	{
		size_t capacityVertCount = m_vertexBuffers[bufferIndex]->getVertexCount();
		if (vb.Size > capacityVertCount)
			m_vertexBuffers[bufferIndex] = createSharedResource(m_engine, imguiCreateVertexBuffer(m_engine, vb.Size));

		auto vbDescriptor = imguiCreateDescriptor<fl::VertexBuffer, ImDrawVert>(vb);
		m_vertexBuffers[bufferIndex]->setBufferAt(*m_engine, 0, std::move(vbDescriptor));
	}

	// Create a new index buffer if the size isn't large enough, then copy the ImGui data into
	// a staging area since Filament's render thread might consume the data at any time.
	{
		size_t capacityIndexCount = m_indexBuffers[bufferIndex]->getIndexCount();
		if (ib.Size > capacityIndexCount)
			m_indexBuffers[bufferIndex] = createSharedResource(m_engine, imguiCreateIndexBuffer(m_engine, ib.Size));

		auto ibDescriptor = imguiCreateDescriptor<fl::IndexBuffer, ImDrawIdx>(ib);
		m_indexBuffers[bufferIndex]->setBuffer(*m_engine, std::move(ibDescriptor));
	}
}

} // namespace spatial::render