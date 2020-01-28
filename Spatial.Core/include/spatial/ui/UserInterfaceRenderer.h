#pragma once

#include <spatial/render/CommonResources.h>
#include <spatial/desktop/Window.h>
#include <vector>
#include <imgui.h>
#include <imgui_internal.h>

#include <filesystem>

namespace spatial::ui
{

class UserInterfaceRenderer
{
private:
    filament::Engine *m_engine;

    render::View m_view;
    render::Scene m_scene;
    render::Camera m_camera;
    render::Material m_material;
    render::EntityResource m_entity;
    render::Texture m_texture;

    std::vector<render::SharedVertexBuffer> m_vertexBuffers;
    std::vector<render::SharedIndexBuffer> m_indexBuffers;
    std::vector<render::SharedMaterialInstance> m_materialInstances;

    void renderDrawData();
    void createBuffers(size_t numRequiredBuffers);
    void createMaterialInstances(size_t numRequiredInstances);
    void populateVertexData(size_t bufferIndex, const ImVector<ImDrawVert> &vb, const ImVector<ImDrawIdx> &ib);

public:
    UserInterfaceRenderer(filament::Engine *engine);
    ~UserInterfaceRenderer();

    void setViewport(int width, int height, float dpiX, float dpiY);

    /**
     * \brief Should be called when applications runs
     */
    void setup(const std::filesystem::path& fontPath);

    /**
     * \brief Should be called every frame start
     * @param delta The delta time in seconds
     */
    void beforeRender(float delta);

    /**
     * \brief Dispatches render commands. Should be called every frame, before the render
     */
    void dispatchCommands();

    //region Getters
    const auto getView()
    {
        return m_view.get();
    }
    //endregion
};

} // namespace spatial::render