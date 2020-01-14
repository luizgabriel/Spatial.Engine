#pragma once

#include <spatial/render/CommonResources.h>
#include <spatial/desktop/Window.h>
#include <vector>
#include <imgui.h>
#include <imgui_internal.h>

namespace spatial::render
{

class UserInterfaceRenderer
{
private:
    filament::Engine *m_engine;

    filament::View *m_view;
    Scene m_scene;
    Camera m_camera;
    Material m_material;
    EntityResource m_entity;
    Texture m_texture;

    std::vector<filament::VertexBuffer *> m_vertexBuffers;
    std::vector<filament::IndexBuffer *> m_indexBuffers;
    std::vector<filament::MaterialInstance *> m_materialInstances;

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
    void setup();

    /**
     * \brief Should be called every frame start
     * @param delta The delta time in seconds
     */
    void beforeRender(float delta);

    /**
     * \brief Dispatches render commands. Should be called every frame, before the render
     */
    void dispatchCommands();

    filament::View *getView()
    {
        return m_view;
    }
};

} // namespace spatial::render