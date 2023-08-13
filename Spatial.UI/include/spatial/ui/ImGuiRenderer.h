#pragma once

#include <imgui.h>
#include <spatial/common/Math.h>
#include <spatial/graphics/MaterialInstance.h>
#include <spatial/graphics/Resources.h>
#include <string_view>
#include <vector>

namespace spatial::ui
{

class ImGuiRenderer
{
  private:
	filament::Engine& mEngine;

	graphics::View mView;
	graphics::Scene mScene;
	graphics::Entity mCameraEntity;
	graphics::Camera mCamera;
	graphics::Entity mEntity;
	graphics::Skybox mSkybox;
	graphics::SharedMaterial mMaterial;
	graphics::SharedTexture mFontTexture;

	std::vector<graphics::VertexBuffer> mVertexBuffers;
	std::vector<graphics::IndexBuffer> mIndexBuffers;
	std::vector<graphics::MaterialInstance> mMaterialInstances;

	ImGuiContext* mImguiContext;

	void renderDrawData();
	void createBuffers(size_t numRequiredBuffers);
	void createMaterialInstances(size_t numRequiredInstances);

	void populateVertexData(size_t bufferIndex, const ImVector<ImDrawVert>& vb);

	void populateIndexData(size_t bufferIndex, const ImVector<ImDrawIdx>& ib);

  public:
	explicit ImGuiRenderer(filament::Engine& engine);
	~ImGuiRenderer();

	ImGuiRenderer(const ImGuiRenderer& other) = delete;
	ImGuiRenderer& operator=(const ImGuiRenderer& other) = delete;

	void setViewport(const math::uvec2& windowSize, const math::uvec2& frameBufferSize);

	void setMaterial(const uint8_t* data, size_t size);

	void addFont(const uint8_t* data, size_t size);

	void createFontTextureAtlas();

	/**
	 * \brief Should be called every frame start
	 * @param delta The delta time in seconds
	 */
	void initNewFrame(float delta);

	/**
	 * \brief Dispatches render commands. Should be called every frame, before the render
	 */
	void drawFrame();

	// region Getters
	[[nodiscard]] const auto& getView() const
	{
		return *mView;
	}
	// endregion
};

} // namespace spatial::ui