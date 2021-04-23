#include <imgui.h>
#include <spatial/render/ResourceLoaders.h>
#include "ImGuiSceneWindow.h"

namespace spatial::editor
{

ImGuiSceneWindow::ImGuiSceneWindow(filament::Engine& engine, math::int2 size)
	: mRenderColorTexture{createTexture(engine, size, filament::backend::TextureFormat::RGBA16F,
										filament::Texture::Usage::COLOR_ATTACHMENT |
										filament::Texture::Usage::SAMPLEABLE)},
	  mRenderDepthTexture{createTexture(engine, size, filament::backend::TextureFormat::DEPTH16,
										filament::Texture::Usage::DEPTH_ATTACHMENT)},
	  mRenderTarget{createRenderTarget(engine, mRenderColorTexture.ref(), mRenderDepthTexture.ref())},
	  mWindowSize{.0f, .0f}
{
}

void ImGuiSceneWindow::draw(const std::string_view windowTitle)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin(windowTitle.begin());
	auto size = ImGui::GetWindowSize();
	size.y -= 25;
	auto currentSize = math::int2{size.x, size.y};

	if (currentSize != mWindowSize) {
		mWindowSize = std::move(currentSize);
		mWindowResizedSignal(mWindowSize);
	}

	ImGui::Image(mRenderColorTexture.get(), size);
	ImGui::End();
	ImGui::PopStyleVar();
}


} // namespace spatial::ui