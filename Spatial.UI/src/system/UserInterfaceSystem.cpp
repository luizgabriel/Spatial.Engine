#include <spatial/ui/components/DockSpace.h>
#include <spatial/ui/system/ImGuiHelpers.h>
#include <spatial/ui/system/UserInterfaceSystem.h>

namespace spatial::ui {

    UserInterfaceSystem::UserInterfaceSystem(filament::Engine &engine) : mRenderer{engine} {
    }

    void UserInterfaceSystem::onStart() {
        auto &io = ImGui::GetIO();
        io.ClipboardUserData = this;

        imguiSetupTheme();
        imguiSetupInput();

        io.SetClipboardTextFn = [](void *userData, const char *text) {
            auto &self = *static_cast<UserInterfaceSystem *>(userData);
            self.mClipboardController.set(text);
        };

        io.GetClipboardTextFn = [](void *userData) -> const char * {
            auto &self = *static_cast<UserInterfaceSystem *>(userData);
            auto text = self.mClipboardController.get();
            if (text) return text->c_str();

            return nullptr;
        };
    }

    void UserInterfaceSystem::onEvent(const WindowResizedEvent &event) {
        getRenderer().setViewport(event.windowSize, event.frameBufferSize);
    }

    void UserInterfaceSystem::onEvent(const MouseMovedEvent &event) {
        imguiSetMousePosition(event.position);
    }

    void UserInterfaceSystem::onEvent(const KeyEvent &event) {
        imguiSetKey(event.key, event.action);
    }

    void UserInterfaceSystem::onEvent(const TextEvent &event) {
        imguiSetText(event.text);
    }

    void UserInterfaceSystem::onEvent(const MouseScrolledEvent &event) {
        imguiSetScrollOffset(event.offset);
    }

    void UserInterfaceSystem::onUpdateFrame(float delta) {
        mRenderer.initNewFrame(delta);

        {
            auto dockSpace = ui::DockSpace{"Spatial"};
            mRenderGuiSignal();
        }

        mRenderer.drawFrame();
    }

    void UserInterfaceSystem::onRender(filament::Renderer &renderer) const {
        renderer.render(&mRenderer.getView());
    }

    void UserInterfaceSystem::setMaterial(FileSystem &fileSystem, std::string_view materialResourcePath) {
        auto materialData = fileSystem.readBinary(materialResourcePath);
        assert(!materialData.empty());
        mRenderer.setMaterial(materialData.data(), materialData.size());
    }

    void UserInterfaceSystem::addFont(FileSystem &fileSystem, std::string_view fontPath) {
        auto fontData = fileSystem.readBinary(fontPath);
        assert(!fontData.empty());
        mRenderer.addFont(fontData.data(), fontData.size());
    }

    void UserInterfaceSystem::setClipboardController(ClipboardController controller) {
        mClipboardController = std::move(controller);
    }

} // namespace spatial::ui