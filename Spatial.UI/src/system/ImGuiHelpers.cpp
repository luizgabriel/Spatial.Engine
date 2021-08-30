#include <spatial/ui/system/ImGuiHelpers.h>
#include <vector>

namespace fl = filament;

namespace spatial::ui
{

void imguiRefreshViewport(float width, float height, float scaleX, float scaleY)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(width, height);
	io.DisplayFramebufferScale.x = scaleX;
	io.DisplayFramebufferScale.y = scaleY;
}

void imguiRefreshDeltaTime(float delta)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = delta;
}

math::float2 imguiGetFrameSize()
{
	ImGuiIO& io = ImGui::GetIO();
	auto fw = io.DisplaySize.x * io.DisplayFramebufferScale.x;
	auto fh = io.DisplaySize.y * io.DisplayFramebufferScale.y;

	return {fw, fh};
}

bool imguiIsMinimized()
{
	auto fs = imguiGetFrameSize();
	return fs.x == 0 && fs.y == 0;
}

render::VertexBuffer imguiCreateVertexBuffer(fl::Engine& engine, uint32_t capacity)
{
	return render::VertexBuffer{engine, fl::VertexBuffer::Builder()
									.vertexCount(capacity)
									.bufferCount(1)
									.attribute(fl::VertexAttribute::POSITION, 0,
											   fl::VertexBuffer::AttributeType::FLOAT2, 0, sizeof(ImDrawVert))
									.attribute(fl::VertexAttribute::UV0, 0, fl::VertexBuffer::AttributeType::FLOAT2,
											   sizeof(fl::math::float2), sizeof(ImDrawVert))
									.attribute(fl::VertexAttribute::COLOR, 0, fl::VertexBuffer::AttributeType::UBYTE4,
											   2 * sizeof(fl::math::float2), sizeof(ImDrawVert))
									.normalized(fl::VertexAttribute::COLOR)
									.build(engine)};
}

render::IndexBuffer imguiCreateIndexBuffer(fl::Engine& engine, uint32_t capacity)
{
	return render::IndexBuffer{
		engine,
		fl::IndexBuffer::Builder().indexCount(capacity).bufferType(fl::IndexBuffer::IndexType::USHORT).build(engine)};
}

render::Texture imguiCreateTextureAtlas(fl::Engine& engine, const uint8_t* data, uint32_t size)
{
	auto& io = ImGui::GetIO();

	auto copiedData = new uint8_t[size];
	std::copy(data, data + size, &copiedData[0]);
	io.Fonts->AddFontFromMemoryTTF(&copiedData[0], static_cast<int>(size), 16.0f);

	unsigned char* imageData;
	int width, height, bpp;
	io.Fonts->GetTexDataAsAlpha8(&imageData, &width, &height, &bpp);

	auto textureSize = width * height * bpp;
	auto pb = fl::Texture::PixelBufferDescriptor{imageData, static_cast<size_t>(textureSize), fl::Texture::Format::R, fl::Texture::Type::UBYTE};
	const auto texture = fl::Texture::Builder()
							 .width(static_cast<uint32_t>(width))
							 .height(static_cast<uint32_t>(height))
							 .levels(static_cast<uint8_t>(1))
							 .format(fl::Texture::InternalFormat::R8)
							 .sampler(fl::Texture::Sampler::SAMPLER_2D)
							 .build(engine);

	texture->setImage(engine, 0, std::move(pb));

	return render::createResource(engine, texture);
}

} // namespace spatial