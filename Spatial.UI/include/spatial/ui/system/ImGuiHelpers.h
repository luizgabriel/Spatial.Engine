#pragma once

#include <filament/IndexBuffer.h>
#include <filament/VertexBuffer.h>
#include <spatial/render/Resources.h>
#include <string_view>
#include <string>
#include <imgui.h>

namespace spatial::ui
{

void imguiRefreshViewport(float width, float height, float scaleX, float scaleY);

void imguiRefreshDeltaTime(float delta);

math::float2 imguiGetFrameSize();

bool imguiIsMinimized();

render::VertexBuffer imguiCreateVertexBuffer(filament::Engine& engine, uint32_t capacity);

render::IndexBuffer imguiCreateIndexBuffer(filament::Engine& engine, uint32_t capacity);

void imguiAddFont(const uint8_t* data, size_t size);

void imguiAddIconFont(const uint8_t* data, size_t size, uint16_t min, uint16_t max);

render::Texture imguiCreateTextureAtlas(filament::Engine& engine);

template <typename ImGuiType>
filament::backend::BufferDescriptor imguiCreateDescriptor(const ImVector<ImGuiType>& imVector)
{
	auto nVec = static_cast<size_t>(imVector.Size);
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