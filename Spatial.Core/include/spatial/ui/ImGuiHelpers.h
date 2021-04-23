#pragma once

#include <filament/IndexBuffer.h>
#include <filament/VertexBuffer.h>
#include <imgui.h>
#include <spatial/render/Resources.h>
#include <string_view>

namespace spatial
{

void imguiRefreshViewport(std::uint32_t width, std::uint32_t height, float scaleX, float scaleY);

void imguiRefreshDeltaTime(float delta);

math::int2 imguiGetFrameSize();

bool imguiIsMinimized();

VertexBuffer imguiCreateVertexBuffer(filament::Engine& engine, size_t capacity);

IndexBuffer imguiCreateIndexBuffer(filament::Engine& engine, size_t capacity);

Texture imguiCreateTextureAtlas(fl::Engine& engine, const std::string& resourceData);

template <typename ImGuiType>
filament::backend::BufferDescriptor imguiCreateDescriptor(const ImVector<ImGuiType>& imVector)
{
	auto nVec = imVector.Size;
	auto alloc = std::allocator<ImGuiType>();
	auto data = alloc.allocate(nVec);

	std::copy(imVector.begin(), imVector.end(), data);

	auto callback = [](void* buffer, size_t size, void*) {
		auto alloc = std::allocator<ImGuiType>();
		auto nVec = size / sizeof(ImGuiType);
		alloc.deallocate(reinterpret_cast<ImGuiType*>(buffer), nVec);
	};

	auto nVecBytes = nVec * sizeof(ImGuiType);
	return {data, nVecBytes, callback, nullptr};
}

} // namespace spatial