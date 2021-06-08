#include "ImGuiSceneWindow.h"
#include "ImGuiWindow.h"
#include <imgui.h>
#include <spatial/render/ResourceLoaders.h>

namespace spatial::editor
{

ImGuiSceneWindow::ImGuiSceneWindow(filament::Engine& engine, math::int2 size)
	: mRenderColorTexture{render::createTexture(engine, size, filament::backend::TextureFormat::RGBA16F,
										filament::Texture::Usage::COLOR_ATTACHMENT |
										filament::Texture::Usage::SAMPLEABLE)},
	  mRenderDepthTexture{render::createTexture(engine, size, filament::backend::TextureFormat::DEPTH16,
										filament::Texture::Usage::DEPTH_ATTACHMENT)},
	  mRenderTarget{render::createRenderTarget(engine, mRenderColorTexture.ref(), mRenderDepthTexture.ref())},
	  mWindowSize{.0f, .0f}
{
}

void ImGuiSceneWindow::draw(const std::string_view windowTitle)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    {
		auto window = ImGuiWindow{windowTitle};
        auto size = window.getSize();
        size.y -= 25.0f;

        if ((static_cast<math::int2>(size)) != mWindowSize) {
            mWindowSize = std::move(size);
            mWindowResizedSignal(mWindowSize);
        }

        ImGui::Image(mRenderColorTexture.get(), ImVec2(size.x, size.y));
	}

	ImGui::PopStyleVar();
}


} // namespace spatial::ui