#include <imgui.h>
#include <spatial/ui/components/Window.h>

namespace spatial::ui
{

Window::Window(const std::string_view name)
{
	ImGui::Begin(name.data(), nullptr, ImGuiWindowFlags_HorizontalScrollbar);
}

Window::~Window()
{
	ImGui::End();
}

math::float2 Window::getSize() const
{
	auto size = ImGui::GetWindowSize();
	return {size.x, size.y};
}

void Window::setSize(const math::float2& size)
{
	ImGui::SetWindowSize(ImVec2(size.x, size.y));
}

math::float2 Window::getPosition() const
{
	auto position = ImGui::GetWindowPos();
	return {position.x, position.y};
}

void Window::setPosition(const math::float2& size)
{
	ImGui::SetWindowPos(ImVec2(size.x, size.y));
}

bool Window::isFocused() const
{
	return ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
}

} // namespace spatial::ui