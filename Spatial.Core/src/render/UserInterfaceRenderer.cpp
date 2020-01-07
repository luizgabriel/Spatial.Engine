#include <spatial/render/UserInterfaceRenderer.h>
#include <spatial/core/Asset.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/RenderableManager.h>

using namespace std::filesystem;
using namespace spatial::core;
using namespace spatial::common;
namespace fl = filament;

namespace spatial::render
{

//region helpers
Material createMaterial(fl::Engine *engine, const path &name)
{
	auto data = Asset::read(path{"materials"} / name);
	return createMaterial(engine, data);
}

void imguiRefreshViewport(std::uint32_t width, std::uint32_t height, float scaleX, float scaleY)
{
	ImGuiIO &io = ImGui::GetIO();
	io.DisplaySize = ImVec2(width, height);
	io.DisplayFramebufferScale.x = scaleX;
	io.DisplayFramebufferScale.y = scaleY;
}

void imguiRefreshDeltaTime(float delta)
{
	ImGuiIO &io = ImGui::GetIO();
	io.DeltaTime = delta;
}

std::pair<int, int> imguiGetFrameSize()
{
	ImGuiIO &io = ImGui::GetIO();
	int fbwidth = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fbheight = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);

	return {fbwidth, fbheight};
}

bool imguiIsMinimized()
{
	auto [width, height] = imguiGetFrameSize();
	return width == 0 && height == 0;
}

SharedVertexBuffer imguiCreateVertexBuffer(fl::Engine *engine, size_t capacity)
{
	auto vb = fl::VertexBuffer::Builder()
				  .vertexCount(capacity)
				  .bufferCount(1)
				  .attribute(fl::VertexAttribute::POSITION, 0, fl::VertexBuffer::AttributeType::FLOAT2, 0,
							 sizeof(ImDrawVert))
				  .attribute(fl::VertexAttribute::UV0, 0, fl::VertexBuffer::AttributeType::FLOAT2,
							 sizeof(fl::math::float2), sizeof(ImDrawVert))
				  .attribute(fl::VertexAttribute::COLOR, 0, fl::VertexBuffer::AttributeType::UBYTE4,
							 2 * sizeof(fl::math::float2), sizeof(ImDrawVert))
				  .normalized(fl::VertexAttribute::COLOR)
				  .build(*engine);

	return createSharedResource(engine, vb);
}

SharedIndexBuffer imguiCreateIndexBuffer(fl::Engine *engine, size_t capacity)
{
	auto ib = fl::IndexBuffer::Builder()
				  .indexCount(capacity)
				  .bufferType(fl::IndexBuffer::IndexType::USHORT)
				  .build(*engine);

	return createSharedResource(engine, ib);
}

uint64_t makeScissorKey(int fbheight, const ImVec4 &clipRect)
{
	uint16_t left = (uint16_t)clipRect.x;
	uint16_t bottom = (uint16_t)(fbheight - clipRect.w);
	uint16_t width = (uint16_t)(clipRect.z - clipRect.x);
	uint16_t height = (uint16_t)(clipRect.w - clipRect.y);
	return ((uint64_t)left << 0ull) |
		   ((uint64_t)bottom << 16ull) |
		   ((uint64_t)width << 32ull) |
		   ((uint64_t)height << 48ull);
}

Texture createTextureAtlas(fl::Engine *engine)
{
	unsigned char *data;
	int width, height;

	auto &io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	io.Fonts->GetTexDataAsAlpha8(&data, &width, &height);

	auto size = (size_t)(width * height);
	auto pb = fl::Texture::PixelBufferDescriptor{data, size, fl::Texture::Format::R, fl::Texture::Type::UBYTE};

	auto texture = fl::Texture::Builder()
					   .width((uint32_t)width)
					   .height((uint32_t)height)
					   .levels((uint8_t)1)
					   .format(fl::Texture::InternalFormat::R8)
					   .sampler(fl::Texture::Sampler::SAMPLER_2D)
					   .build(*engine);
	texture->setImage(*engine, 0, std::move(pb));

	return {engine, texture};
}
//endregion

UserInterfaceRenderer::UserInterfaceRenderer(fl::Engine *engine)
	: m_engine{engine},
	  m_scene{createScene(m_engine)},
	  m_view{createView(m_engine)},
	  m_camera{createCamera(m_engine)},
	  m_material{createMaterial(m_engine, path{"ui_blit.filamat"})},
	  m_entity{m_engine},
	  m_texture{m_engine}
{
	m_view->setCamera(m_camera);
	m_view->setScene(m_scene);
	m_scene->addEntity(m_entity.get());

	ImGui::CreateContext();

	//Should be called after ImGui::CreateContext()
	m_texture = createTextureAtlas(m_engine);

	auto sampler = fl::TextureSampler{fl::TextureSampler::MinFilter::LINEAR, fl::TextureSampler::MagFilter::LINEAR};
	m_material->setDefaultParameter("albedo", m_texture, sampler);

	ImGui::StyleColorsDark();
}

UserInterfaceRenderer::~UserInterfaceRenderer()
{
	ImGui::DestroyContext();
}

void UserInterfaceRenderer::setViewport(std::uint32_t width, std::uint32_t height, float dpiX, float dpiY)
{
	m_view->setViewport({0, 0, width, height});
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

void UserInterfaceRenderer::render()
{
	// Avoid rendering when minimized and scale coordinates for retina displays.
	if (imguiIsMinimized())
		return;

	ImGui::Render();

	renderDrawData(ImGui::GetDrawData());
}

void UserInterfaceRenderer::renderDrawData(ImDrawData *imguiData)
{
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
			scissorRects[makeScissorKey(fbheight, pcmd.ClipRect)] = nullptr;
		}
	}
	auto rbuilder = fl::RenderableManager::Builder(nPrims);
	rbuilder.boundingBox({{0, 0, 0}, {10000, 10000, 10000}}).culling(false);

	// Ensure that we have a material instance for each scissor rectangle.
	size_t previousSize = m_materialInstances.size();
	if (scissorRects.size() > m_materialInstances.size())
	{
		m_materialInstances.resize(scissorRects.size());
		for (size_t i = previousSize; i < m_materialInstances.size(); i++)
		{
			m_materialInstances[i] = createSharedResource(m_engine, m_material->createInstance());
		}
	}

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
		populateVertexData(bufferIndex,
						   cmds->VtxBuffer.Size * sizeof(ImDrawVert), cmds->VtxBuffer.Data,
						   cmds->IdxBuffer.Size * sizeof(ImDrawIdx), cmds->IdxBuffer.Data);
		for (const auto &pcmd : cmds->CmdBuffer)
		{
			if (pcmd.UserCallback)
			{
				pcmd.UserCallback(cmds, &pcmd);
			}
			else
			{
				uint64_t skey = makeScissorKey(fbheight, pcmd.ClipRect);
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

void UserInterfaceRenderer::createBuffers(int numRequiredBuffers)
{
	if (numRequiredBuffers > m_vertexBuffers.size())
	{
		size_t previousSize = m_vertexBuffers.size();
		m_vertexBuffers.resize(numRequiredBuffers, nullptr);
		for (size_t i = previousSize; i < m_vertexBuffers.size(); i++)
		{
			// Pick a reasonable starting capacity; it will grow if needed.
			m_vertexBuffers[i] = imguiCreateVertexBuffer(m_engine, 1000);
		}
	}
	if (numRequiredBuffers > m_indexBuffers.size())
	{
		size_t previousSize = m_indexBuffers.size();
		m_indexBuffers.resize(numRequiredBuffers, nullptr);
		for (size_t i = previousSize; i < m_indexBuffers.size(); i++)
		{
			// Pick a reasonable starting capacity; it will grow if needed.
			m_indexBuffers[i] = imguiCreateIndexBuffer(m_engine, 5000);
		}
	}
}

void UserInterfaceRenderer::populateVertexData(size_t bufferIndex, size_t vbSizeInBytes, void *vbImguiData,
											  size_t ibSizeInBytes, void *ibImguiData)
{
	// Create a new vertex buffer if the size isn't large enough, then copy the ImGui data into
	// a staging area since Filament's render thread might consume the data at any time.
	size_t requiredVertCount = vbSizeInBytes / sizeof(ImDrawVert);
	size_t capacityVertCount = m_vertexBuffers[bufferIndex]->getVertexCount();
	if (requiredVertCount > capacityVertCount)
	{
		m_vertexBuffers[bufferIndex] = imguiCreateVertexBuffer(m_engine, requiredVertCount);
	}

	size_t nVbBytes = requiredVertCount * sizeof(ImDrawVert);
	void *vbFilamentData = malloc(nVbBytes);
	memcpy(vbFilamentData, vbImguiData, nVbBytes);
	m_vertexBuffers[bufferIndex]->setBufferAt(*m_engine, 0,
											  fl::VertexBuffer::BufferDescriptor(vbFilamentData, nVbBytes,
																				 [](void *buffer, size_t size, void *user) {
																					 free(buffer);
																				 },
																				 /* user = */ nullptr));

	// Create a new index buffer if the size isn't large enough, then copy the ImGui data into
	// a staging area since Filament's render thread might consume the data at any time.
	size_t requiredIndexCount = ibSizeInBytes / 2;
	size_t capacityIndexCount = m_indexBuffers[bufferIndex]->getIndexCount();
	if (requiredIndexCount > capacityIndexCount)
	{
		m_indexBuffers[bufferIndex] = imguiCreateIndexBuffer(m_engine, requiredIndexCount);
	}

	size_t nIbBytes = requiredIndexCount * 2;
	void *ibFilamentData = malloc(nIbBytes);
	memcpy(ibFilamentData, ibImguiData, nIbBytes);
	m_indexBuffers[bufferIndex]->setBuffer(*m_engine,
										   fl::IndexBuffer::BufferDescriptor(ibFilamentData, nIbBytes,
																			 [](void *buffer, size_t size, void *user) {
																				 free(buffer);
																			 },
																			 /* user = */ nullptr));
}

} // namespace spatial::render