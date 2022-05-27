#pragma once

#include <filament/IndexBuffer.h>
#include <filament/VertexBuffer.h>
#include <imgui.h>
#include <spatial/common/Key.h>
#include <spatial/graphics/Resources.h>
#include <string>
#include <string_view>

namespace spatial::ui
{

void imguiRefreshViewport(float width, float height, float scaleX, float scaleY);

void imguiRefreshDeltaTime(float delta);

math::vec2 imguiGetFrameSize();

bool imguiIsMinimized();

graphics::VertexBuffer imguiCreateVertexBuffer(filament::Engine& engine, uint32_t capacity);

graphics::IndexBuffer imguiCreateIndexBuffer(filament::Engine& engine, uint32_t capacity);

void imguiAddFont(const uint8_t* data, size_t size, float fontSize);

graphics::Texture imguiCreateTextureAtlas(filament::Engine& engine);

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

void imguiSetupInput();

void imguiSetMousePosition(filament::math::float2 position);

void imguiSetKey(Key key, KeyAction action);

void imguiSetText(std::string_view text);

void imguiSetScrollOffset(double xOffset, double yOffset);

void imguiSetupTheme();

} // namespace spatial::ui