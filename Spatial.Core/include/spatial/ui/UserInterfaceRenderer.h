#pragma once

#include <spatial/render/CommonResources.h>
#include <vector>
#include <imgui.h>

#include <filesystem>

namespace spatial
{

class UserInterfaceRenderer
{
private:
	filament::Engine* m_engine;

	View m_view;
	Scene m_scene;
	Camera m_camera;
	Material m_material;
	Entity m_entity;
	Texture m_texture;

	std::vector<SharedVertexBuffer> m_vertexBuffers;
	std::vector<SharedIndexBuffer> m_indexBuffers;
	std::vector<SharedMaterialInstance> m_materialInstances;

	ImGuiContext* m_imguiContext;

	void renderDrawData();
	void createBuffers(size_t numRequiredBuffers);
	void createMaterialInstances(size_t numRequiredInstances);
	void populateVertexData(size_t bufferIndex, const ImVector<ImDrawVert>& vb, const ImVector<ImDrawIdx>& ib);

public:
	explicit UserInterfaceRenderer(filament::Engine* engine);
	~UserInterfaceRenderer();

	UserInterfaceRenderer(const UserInterfaceRenderer& other) = delete;

	UserInterfaceRenderer(UserInterfaceRenderer&& other) noexcept;

	UserInterfaceRenderer& operator=(const UserInterfaceRenderer& other) = delete;

	UserInterfaceRenderer& operator=(UserInterfaceRenderer&& other) noexcept;

	void setViewport(int width, int height, float dpiX, float dpiY);

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
	auto getView() { return m_view.get(); }
	// endregion
};

} // namespace spatial