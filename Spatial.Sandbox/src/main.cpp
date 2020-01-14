#include <spatial/spatial.h>

#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>


namespace fl = filament;
using namespace spatial;
using namespace spatial::math;
using namespace spatial::desktop;
using namespace spatial::core;
using namespace spatial::common;
using namespace spatial::render;
using namespace spatial::input;
using namespace std::filesystem;

namespace ImGui
{

void SpatialDockLayout(ImGuiID dockspaceId)
{
    ImGui::DockBuilderRemoveNode(dockspaceId); // Clear out existing layout
    ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);

    auto dockId = dockspaceId;
    auto dockRight = ImGui::DockBuilderSplitNode(dockId, ImGuiDir_Right, 0.20f, NULL, &dockId);
    //auto dockBottom = ImGui::DockBuilderSplitNode(dockId, ImGuiDir_Down, 0.20f, NULL, &dockId);

    ImGui::DockBuilderDockWindow("Properties", dockRight);
    ImGui::DockBuilderFinish(dockspaceId);
}

bool BeginSpatialEngine(bool *openedPropertiesPtr)
{
    static ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_PassthruCentralNode;
    static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGuiViewport *viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", nullptr, windowFlags);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiID dockspaceId = ImGui::GetID("SpatialDockSpace");

    if (ImGui::DockBuilderGetNode(dockspaceId) == NULL)
    {
        ImGui::SpatialDockLayout(dockspaceId);
    }

    ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockFlags);

    if (ImGui::BeginMainMenuBar())
    {
        ImGui::Text("Spatial Engine");
        ImGui::Separator();

        if (ImGui::BeginMenu("Options"))
        {
            ImGui::MenuItem("Properties", NULL, openedPropertiesPtr);
            //ImGui::MenuItem("Console", NULL, &gOpenedLogging);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (*openedPropertiesPtr)
    {
        if (ImGui::Begin("Properties", openedPropertiesPtr))
        {

        }
        ImGui::End();
    }

    return true;
}

} // namespace ImGui

class Sandbox
{
private:
    SignalsConnector<Sandbox> m_signalsConnector;

    fl::Engine *m_engine;
    fl::View *m_view;
    fl::Camera *m_camera;

    Scene m_scene;
    Material m_material;
    MaterialInstance m_instance;

    EntityResource m_light;

    bool m_showEngineGui;
    bool m_openedPropertiesWindow;

public:
    Sandbox(Application *app, RenderingSystem* rendering)
        : m_signalsConnector{app, this},

          m_engine{rendering->getEngine()},
          m_view{rendering->getMainView()},
          m_camera{rendering->getMainCamera()},

          m_scene{createScene(m_engine)},
          m_material{createMaterial(m_engine, Asset::read(path{"materials"} / "plastic.filamat"))},
          m_instance{m_engine, m_material->createInstance()},

          m_light{createEntity(m_engine)},
          m_showEngineGui{true},
          m_openedPropertiesWindow{true}
    {
    }

    void onStart()
    {
        float3 eye{0, 0, 4}, center{0, 0, 0}, up{0, 1, 0};
        m_camera->lookAt(eye, center, up);

        m_view->setScene(m_scene.get());

        m_instance->setParameter("baseColor", fl::RgbType::sRGB, {0.8, 0.0, 0.0});
        m_instance->setParameter("roughness", 0.5f);
        m_instance->setParameter("clearCoat", 1.0f);
        m_instance->setParameter("clearCoatRoughness", 0.3f);

        fl::LightManager::Builder(fl::LightManager::Type::SUN)
            .color(fl::Color::toLinear<fl::ACCURATE>({0.98f, 0.92f, 0.89f}))
            .intensity(110000.0f)
            .direction({0.6f, -1.0f, -0.8f})
            .castShadows(true)
            .sunAngularRadius(1.9f)
            .sunHaloSize(10.0f)
            .sunHaloFalloff(80.0f)
            .build(*m_engine, m_light.get());

        m_scene->addEntity(m_light.get());
    }

    void onUpdateFrame(float delta)
    {
        if (Input::released(Key::G))
            m_showEngineGui = !m_showEngineGui;

        if (m_showEngineGui && ImGui::BeginSpatialEngine(&m_openedPropertiesWindow))
        {
            ImGui::End();
        }
    }
};

int main(int arc, char *argv[])
{
    Asset::init(path{argv[0]}.parent_path() / "assets");

    auto app = Application{};
    auto window = app.getWindowContext().createWindow(1280, 720, "Spatial Engine");
    auto rendering = RenderingSystem{&app, std::move(window)};
    auto input = InputSystem{&app};
    auto sandbox = Sandbox{&app, &rendering};
    auto ui = UserInterfaceSystem(&app, &rendering);

    return app.run();
}