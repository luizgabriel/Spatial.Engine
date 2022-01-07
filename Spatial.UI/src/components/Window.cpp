#include <imgui.h>
#include <spatial/ui/components/Window.h>

namespace spatial::ui
{

Window::Window(const std::string_view name, WindowFlags flags)
{
	const auto imFlags = static_cast<ImGuiWindowFlags>(flags) | ImGuiWindowFlags_AlwaysAutoResize;
	mIsOpen = ImGui::Begin(name.data(), nullptr, imFlags);
}

Window::~Window()
{
	ImGui::End();
}

math::float2 Window::getSize() const
{
	if (!mIsOpen)
		return {.0f};
	auto size = ImGui::GetWindowSize();
	return {size.x, size.y};
}

void Window::setSize(const math::float2& size)
{
	if (isOpen())
		ImGui::SetWindowSize(ImVec2(size.x, size.y));
}

math::float2 Window::getPosition() const
{
	auto position = ImGui::GetWindowPos();
	return {position.x, position.y};
}

void Window::setPosition(const math::float2& size)
{
	if (isOpen())
		ImGui::SetWindowPos(ImVec2(size.x, size.y));
}

bool Window::isFocused() const
{
	return isOpen() && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
}

bool Window::isOpen() const
{
	return mIsOpen;
}

WindowFlags operator|(WindowFlags l, WindowFlags r)
{
	using utype_t = std::underlying_type_t<WindowFlags>;
	return static_cast<WindowFlags>(static_cast<utype_t>(l) | static_cast<utype_t>(r));
}

} // namespace spatial::ui