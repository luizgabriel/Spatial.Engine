#pragma once

#include <spatial/render/CommonResources.h>
#include <spatial/desktop/PlatformEvent.h>
#include <vector>
#include <memory>
#include <imgui.h>

namespace spatial::render
{

class UserInterfaceRenderer
{
private:
    filament::Engine* m_engine;

    Scene m_scene;
    View m_view;
    Camera m_camera;
    Material m_material;
    EntityResource m_entity;
    Texture m_texture;

    std::vector<SharedVertexBuffer> m_vertexBuffers;
    std::vector<SharedIndexBuffer> m_indexBuffers;
    std::vector<SharedMaterialInstance> m_materialInstances;

    void renderDrawData(ImDrawData *imguiData);
    void createBuffers(int numRequiredBuffers);
    void populateVertexData(size_t bufferIndex, const ImVector<ImDrawVert> &vb, const ImVector<ImDrawIdx> &ib);

public:
    UserInterfaceRenderer(filament::Engine *engine);
    ~UserInterfaceRenderer();

    void onStart();

    void setViewport(std::uint32_t width, std::uint32_t height, float dpiX, float dpiY);
    void setNativeWindow(void *handle);

    void beforeRender(float delta);
    void render();

    const filament::View* getView() const
    {
        return m_view.get();
    }
};

} // namespace spatial::render