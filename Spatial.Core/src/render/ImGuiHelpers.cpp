#include <spatial/render/ImGuiHelpers.h>
#include <spatial/core/Asset.h>

#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>

namespace fs = std::filesystem;
namespace fl = filament;

using namespace spatial::core;

namespace spatial::render
{

Material createMaterial(fl::Engine *engine, const fs::path &name)
{
	auto data = Asset::read(fs::path{"materials"} / name);
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

filament::VertexBuffer *imguiCreateVertexBuffer(fl::Engine *engine, size_t capacity)
{
	return fl::VertexBuffer::Builder()
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
}

filament::IndexBuffer *imguiCreateIndexBuffer(fl::Engine *engine, size_t capacity)
{
	return fl::IndexBuffer::Builder()
		.indexCount(capacity)
		.bufferType(fl::IndexBuffer::IndexType::USHORT)
		.build(*engine);
}

uint64_t imguiMakeScissorKey(int fbheight, const ImVec4 &clipRect)
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

Texture imguiCreateTextureAtlas(fl::Engine *engine)
{
	auto &io = ImGui::GetIO();

	auto fontPath = Asset::absolute(fs::path{"fonts"} / "Roboto-Medium.ttf");
	if (fs::exists(fontPath))
	{
		auto filePath = fontPath.generic_string();
		io.Fonts->AddFontFromFileTTF(filePath.c_str(), 16.0f);
	}

	unsigned char *data;
	int width, height, bpp;

	io.Fonts->GetTexDataAsAlpha8(&data, &width, &height, &bpp);

	auto size = size_t(width * height * bpp);
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

} // namespace spatial::render