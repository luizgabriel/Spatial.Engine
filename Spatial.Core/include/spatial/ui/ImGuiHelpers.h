#pragma once

#include <imgui.h>
#include <spatial/render/Resources.h>
#include <string_view>

namespace spatial
{

void imguiRefreshViewport(std::uint32_t width, std::uint32_t height, float scaleX, float scaleY);

void imguiRefreshDeltaTime(float delta);

std::pair<int, int> imguiGetFrameSize();

bool imguiIsMinimized();

SharedVertexBuffer imguiCreateVertexBuffer(filament::Engine& engine, size_t capacity);

SharedIndexBuffer imguiCreateIndexBuffer(filament::Engine& engine, size_t capacity);

uint64_t imguiMakeScissorKey(int frameBufferHeight, const ImVec4& clipRect);

Texture imguiCreateTextureAtlas(fl::Engine& engine, const std::vector<char>& resourceData);

template <typename FilamentType, typename ImGuiType>
filament::backend::BufferDescriptor imguiCreateDescriptor(const ImVector<ImGuiType>& imVector)
{
	auto nVec = imVector.Size;
	auto alloc = std::allocator<ImGuiType>();
	auto data = alloc.allocate(nVec);

	std::copy(imVector.begin(), imVector.end(), data);

	auto callback = [](void* buffer, size_t size, void* user) {
		auto alloc = std::allocator<ImGuiType>();
		auto nVec = size / sizeof(ImGuiType);
		alloc.deallocate(reinterpret_cast<ImGuiType*>(buffer), nVec);
	};

	auto nVecBytes = nVec * sizeof(ImGuiType);
	return {data, nVecBytes, callback, nullptr};
}

} // namespace spatial