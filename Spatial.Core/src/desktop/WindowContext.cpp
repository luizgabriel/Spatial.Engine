#include <boost/locale.hpp>
#include <spatial/common/Key.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>
#include <sstream>

#if defined(SPATIAL_PLATFORM_OSX)
#define GLFW_EXPOSE_NATIVE_COCOA
#elif defined(SPATIAL_PLATFORM_WINDOWS)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(SPATIAL_PLATFORM_UNIX)
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3native.h>

namespace spatial::desktop
{

bool PlatformContext::sValid{false};
EventQueue PlatformContext::sEventQueue{};

PlatformContext::PlatformContext()
{
	if (!sValid)
		sValid = glfwInit() == GLFW_TRUE;

	glfwWindowHint(GLFW_SAMPLES, GLFW_DONT_CARE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

PlatformContext::~PlatformContext()
{
	if (sValid)
		glfwTerminate();
}

void PlatformContext::setupCallbacks(GLFWwindow* window)
{
	if (!sValid)
		return;

	glfwSetWindowCloseCallback(window, [](auto* win) { sEventQueue.enqueue<WindowClosedEvent>(); });

	glfwSetCharCallback(window, [](auto* win, unsigned int codepoint) {
		auto ss = std::wstringstream{};
		ss << static_cast<wchar_t>(codepoint);

		sEventQueue.enqueue<TextEvent>(boost::locale::conv::utf_to_utf<char>(ss.str()));
	});

	glfwSetKeyCallback(window, [](auto* win, int key, int scancode, int action, int mods) {
		const auto spatialKey = mapKeyFromScancode(key);
		const auto spatialAction = mapActionFromCode(action);
		sEventQueue.enqueue<KeyEvent>(spatialKey, spatialAction);
	});

	glfwSetMouseButtonCallback(window, [](auto* win, int button, int action, int mods) {
		const auto spatialKey = mapKeyFromMouseButton(button);
		const auto spatialAction = mapActionFromCode(action);
		sEventQueue.enqueue<KeyEvent>(spatialKey, spatialAction);
	});

	glfwSetScrollCallback(window, [](auto* win, double xOffset, double yOffset) {
		sEventQueue.enqueue<MouseScrolledEvent>(xOffset, yOffset);
	});

	glfwSetScrollCallback(window, [](auto* win, double xOffset, double yOffset) {
		sEventQueue.enqueue<MouseScrolledEvent>(xOffset, yOffset);
	});

	glfwSetWindowSizeCallback(window, [](auto* win, int width, int height) {
		int fbw, fbh;
		glfwGetFramebufferSize(win, &fbw, &fbh);
		sEventQueue.enqueue<WindowResizedEvent>(math::int2{width, height}, math::int2{fbw, fbh});
	});

	glfwSetCursorPosCallback(
		window, [](auto* win, double xPos, double yPos) { sEventQueue.enqueue<MouseMovedEvent>(xPos, yPos); });
}

void PlatformContext::onStartFrame(float)
{
	if (!sValid)
		return;

	glfwPollEvents();

	sEventQueue.update<WindowResizedEvent>();
	sEventQueue.update<MouseMovedEvent>();
	sEventQueue.update();
}

Window PlatformContext::createWindow(std::uint16_t width, std::uint16_t height, std::string_view title) const noexcept
{
	auto window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
	setupCallbacks(window);
	return Window{window};
}

Key mapKeyFromScancode(const int scanCode) noexcept
{
	switch (scanCode)
	{
	case GLFW_KEY_LEFT_SHIFT:
		return Key::LShift;
	case GLFW_KEY_RIGHT_SHIFT:
		return Key::RShift;
	case GLFW_KEY_LEFT_CONTROL:
		return Key::LControl;
	case GLFW_KEY_RIGHT_CONTROL:
		return Key::RControl;
	case GLFW_KEY_LEFT_ALT:
		return Key::LAlt;
	case GLFW_KEY_RIGHT_ALT:
		return Key::RAlt;
	case GLFW_KEY_RIGHT_SUPER:
	case GLFW_KEY_LEFT_SUPER:
		return Key::System;
	case GLFW_KEY_MENU:
		return Key::Menu;
	case GLFW_KEY_ESCAPE:
		return Key::Escape;
	case GLFW_KEY_SEMICOLON:
		return Key::Semicolon;
	case GLFW_KEY_SLASH:
		return Key::Slash;
	case GLFW_KEY_EQUAL:
		return Key::Equal;
	case GLFW_KEY_MINUS:
		return Key::Hyphen;
	case GLFW_KEY_LEFT_BRACKET:
		return Key::LBracket;
	case GLFW_KEY_RIGHT_BRACKET:
		return Key::RBracket;
	case GLFW_KEY_COMMA:
		return Key::Comma;
	case GLFW_KEY_PERIOD:
		return Key::Period;
	case GLFW_KEY_APOSTROPHE:
		return Key::Quote;
	case GLFW_KEY_BACKSLASH:
		return Key::Backslash;
	case GLFW_KEY_GRAVE_ACCENT:
		return Key::Tilde;
	case GLFW_KEY_SPACE:
		return Key::Space;
	case GLFW_KEY_ENTER:
		return Key::Enter;
	case GLFW_KEY_BACKSPACE:
		return Key::Backspace;
	case GLFW_KEY_TAB:
		return Key::Tab;
	case GLFW_KEY_PAGE_UP:
		return Key::PageUp;
	case GLFW_KEY_PAGE_DOWN:
		return Key::PageDown;
	case GLFW_KEY_END:
		return Key::End;
	case GLFW_KEY_HOME:
		return Key::Home;
	case GLFW_KEY_INSERT:
		return Key::Insert;
	case GLFW_KEY_DELETE:
		return Key::Delete;
	case GLFW_KEY_KP_ADD:
		return Key::Add;
	case GLFW_KEY_KP_SUBTRACT:
		return Key::Subtract;
	case GLFW_KEY_KP_MULTIPLY:
		return Key::Multiply;
	case GLFW_KEY_KP_DIVIDE:
		return Key::Divide;
	case GLFW_KEY_PAUSE:
		return Key::Pause;
	case GLFW_KEY_F1:
		return Key::F1;
	case GLFW_KEY_F2:
		return Key::F2;
	case GLFW_KEY_F3:
		return Key::F3;
	case GLFW_KEY_F4:
		return Key::F4;
	case GLFW_KEY_F5:
		return Key::F5;
	case GLFW_KEY_F6:
		return Key::F6;
	case GLFW_KEY_F7:
		return Key::F7;
	case GLFW_KEY_F8:
		return Key::F8;
	case GLFW_KEY_F9:
		return Key::F9;
	case GLFW_KEY_F10:
		return Key::F10;
	case GLFW_KEY_F11:
		return Key::F11;
	case GLFW_KEY_F12:
		return Key::F12;
	case GLFW_KEY_F13:
		return Key::F13;
	case GLFW_KEY_F14:
		return Key::F14;
	case GLFW_KEY_F15:
		return Key::F15;
	case GLFW_KEY_LEFT:
		return Key::Left;
	case GLFW_KEY_RIGHT:
		return Key::Right;
	case GLFW_KEY_UP:
		return Key::Up;
	case GLFW_KEY_DOWN:
		return Key::Down;
	case GLFW_KEY_KP_0:
		return Key::Numpad0;
	case GLFW_KEY_KP_1:
		return Key::Numpad1;
	case GLFW_KEY_KP_2:
		return Key::Numpad2;
	case GLFW_KEY_KP_3:
		return Key::Numpad3;
	case GLFW_KEY_KP_4:
		return Key::Numpad4;
	case GLFW_KEY_KP_5:
		return Key::Numpad5;
	case GLFW_KEY_KP_6:
		return Key::Numpad6;
	case GLFW_KEY_KP_7:
		return Key::Numpad7;
	case GLFW_KEY_KP_8:
		return Key::Numpad8;
	case GLFW_KEY_KP_9:
		return Key::Numpad9;
	case GLFW_KEY_A:
		return Key::A;
	case GLFW_KEY_B:
		return Key::B;
	case GLFW_KEY_C:
		return Key::C;
	case GLFW_KEY_D:
		return Key::D;
	case GLFW_KEY_E:
		return Key::E;
	case GLFW_KEY_F:
		return Key::F;
	case GLFW_KEY_G:
		return Key::G;
	case GLFW_KEY_H:
		return Key::H;
	case GLFW_KEY_I:
		return Key::I;
	case GLFW_KEY_J:
		return Key::J;
	case GLFW_KEY_K:
		return Key::K;
	case GLFW_KEY_L:
		return Key::L;
	case GLFW_KEY_M:
		return Key::M;
	case GLFW_KEY_N:
		return Key::N;
	case GLFW_KEY_O:
		return Key::O;
	case GLFW_KEY_P:
		return Key::P;
	case GLFW_KEY_Q:
		return Key::Q;
	case GLFW_KEY_R:
		return Key::R;
	case GLFW_KEY_S:
		return Key::S;
	case GLFW_KEY_T:
		return Key::T;
	case GLFW_KEY_U:
		return Key::U;
	case GLFW_KEY_V:
		return Key::V;
	case GLFW_KEY_W:
		return Key::W;
	case GLFW_KEY_X:
		return Key::X;
	case GLFW_KEY_Y:
		return Key::Y;
	case GLFW_KEY_Z:
		return Key::Z;
	case GLFW_KEY_0:
		return Key::Num0;
	case GLFW_KEY_1:
		return Key::Num1;
	case GLFW_KEY_2:
		return Key::Num2;
	case GLFW_KEY_3:
		return Key::Num3;
	case GLFW_KEY_4:
		return Key::Num4;
	case GLFW_KEY_5:
		return Key::Num5;
	case GLFW_KEY_6:
		return Key::Num6;
	case GLFW_KEY_7:
		return Key::Num7;
	case GLFW_KEY_8:
		return Key::Num8;
	case GLFW_KEY_9:
		return Key::Num9;
	default:
		return Key::UnknownKey;
	}
}

Key mapKeyFromMouseButton(int mouseButton) noexcept
{
	switch (mouseButton)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		return Key::MouseLeft;
	case GLFW_MOUSE_BUTTON_RIGHT:
		return Key::MouseRight;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		return Key::MouseMiddle;
	default:
		return Key::UnknownKey;
	}
}

KeyAction mapActionFromCode(int action) noexcept
{
	switch (action)
	{
	case GLFW_PRESS:
		return KeyAction::Pressed;
	case GLFW_RELEASE:
		return KeyAction::Released;
	default:
		return KeyAction::UnknownAction;
	}
}

} // namespace spatial::desktop
