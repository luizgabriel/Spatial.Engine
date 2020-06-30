#pragma once

#include <spatial/render/Resources.h>
#include <vector>
#include <imgui.h>

#include <filesystem>

namespace spatial
{

class UserInterfaceRenderer
{
private:
	filament::Engine& mEngine;

	SharedView mView;
	Scene mScene;
	Camera mCamera;
	Material mMaterial;
	Entity mEntity;
	Texture mTexture;

	std::vector<SharedVertexBuffer> m_vertexBuffers;
	std::vector<SharedIndexBuffer> m_indexBuffers;
	std::vector<SharedMaterialInstance> m_materialInstances;

	ImGuiContext* m_imguiContext;

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

	/**
	 * \brief Should be called when applications runs
	 */
	void setup(const std::filesystem::path& fontPath);

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