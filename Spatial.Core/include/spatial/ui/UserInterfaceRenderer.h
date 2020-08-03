#pragma once

#include <imgui.h>
#include <spatial/render/Resources.h>
#include <vector>

#include <filesystem>
#include <string_view>

namespace spatial
{

class UserInterfaceRenderer
{
  private:
	filament::Engine& mEngine;

	SharedView mView;
	Scene mScene;
	Entity mCameraEntity;
	Camera mCamera;
	Material mMaterial;
	Entity mEntity;
	Skybox mSkybox;
	Texture mFontTexture;

	std::vector<VertexBuffer> mVertexBuffers;
	std::vector<IndexBuffer> mIndexBuffers;
	std::vector<MaterialInstance> mMaterialInstances;

	ImGuiContext* mImguiContext;

	void renderDrawData();
	void createBuffers(size_t numRequiredBuffers);
	void createMaterialInstances(size_t numRequiredInstances);
	void populateVertexData(size_t bufferIndex, const ImVector<ImDrawVert>& vb, const ImVector<ImDrawIdx>& ib);

  public:
	explicit UserInterfaceRenderer(filament::Engine& engine);
	~UserInterfaceRenderer();

	UserInterfaceRenderer(const UserInterfaceRenderer& other) = delete;
	UserInterfaceRenderer& operator=(const UserInterfaceRenderer& other) = delete;

	void setViewport(const std::pair<int, int>& windowSize, const std::pair<int, int>& frameBufferSize);

	void setMaterial(const std::vector<char>& materialData);

	void setFont(const std::vector<char>& fontData);

	void setupEngineTheme();

	/**
	 * \brief Should be called every frame start
	 * @param delta The delta time in seconds
	 */
	void beforeRender(float delta) const;

	/**
	 * \brief Dispatches render commands. Should be called every frame, before the render
	 */
	void dispatchCommands();

	// region Getters
	auto& getView()
	{
		return mView;
	}
	// endregion
};

} // namespace spatial