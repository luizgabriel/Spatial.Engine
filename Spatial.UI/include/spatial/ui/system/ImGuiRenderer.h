#pragma once

#include <imgui.h>
#include <spatial/common/Math.h>
#include <spatial/render/Resources.h>
#include <vector>

#include <filesystem>
#include <string_view>

namespace spatial
{

class ImGuiRenderer
{
  private:
	filament::Engine& mEngine;

	render::View mView;
	render::Scene mScene;
	render::Entity mCameraEntity;
	render::Camera mCamera;
	render::Entity mEntity;
	render::Skybox mSkybox;
	render::SharedMaterial mMaterial;
	render::Texture mFontTexture;

	std::vector<render::VertexBuffer> mVertexBuffers;
	std::vector<render::IndexBuffer> mIndexBuffers;
	std::vector<render::MaterialInstance> mMaterialInstances;

	ImGuiContext* mImguiContext;

	void renderDrawData();
	void createBuffers(size_t numRequiredBuffers);
	void createMaterialInstances(size_t numRequiredInstances);
	void populateVertexData(size_t bufferIndex, const ImVector<ImDrawVert>& vb, const ImVector<ImDrawIdx>& ib);

  public:
	explicit ImGuiRenderer(filament::Engine& engine);
	~ImGuiRenderer();

	ImGuiRenderer(const ImGuiRenderer& other) = delete;
	ImGuiRenderer& operator=(const ImGuiRenderer& other) = delete;

	void setViewport(const math::float2& windowSize, const math::float2& frameBufferSize);

	void setMaterial(const uint8_t* data, size_t size);

	void addFont(const uint8_t* data, size_t size);

	void createFontTextureAtlas();

	void setupEngineTheme();

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

} // namespace spatial