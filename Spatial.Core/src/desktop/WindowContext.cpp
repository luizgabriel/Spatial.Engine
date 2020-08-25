// ReSharper disable CppMemberFunctionMayBeStatic
#include <spatial/common/Key.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>

namespace spatial
{

DesktopPlatformContext::DesktopPlatformContext() : mValid{true}, mEventQueue{}
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		mValid = false;
}

DesktopPlatformContext::~DesktopPlatformContext()
{
	if (mValid)
		SDL_Quit();
}

void DesktopPlatformContext::onStartFrame(float)
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			mEventQueue.enqueue<WindowClosedEvent>();
			break;

		case SDL_TEXTINPUT:
			mEventQueue.enqueue<TextEvent>(std::string{e.text.text});

		case SDL_KEYDOWN: {
			auto key = mapKeyFromScancode(e.key.keysym.scancode);
			mEventQueue.enqueue<KeyEvent>(key, KeyAction::Pressed, e.key.repeat);

			break;
		}

		case SDL_KEYUP: {
			auto key = mapKeyFromScancode(e.key.keysym.scancode);
			mEventQueue.enqueue<KeyEvent>(key, KeyAction::Released, e.key.repeat);
			break;
		}

		case SDL_MOUSEWHEEL:
			mEventQueue.enqueue<MouseScrolledEvent>(e.wheel.x, e.wheel.y);
			break;

		case SDL_MOUSEBUTTONDOWN:
			mEventQueue.enqueue<KeyEvent>(mapKeyFromMouseButton(e.button.button), KeyAction::Pressed, e.button.clicks);
			break;

		case SDL_MOUSEBUTTONUP:
			mEventQueue.enqueue<KeyEvent>(mapKeyFromMouseButton(e.button.button), KeyAction::Released, e.button.clicks);
			break;

		case SDL_MOUSEMOTION:
			mEventQueue.enqueue<MouseMovedEvent>(e.motion.x, e.motion.y);
			break;

		case SDL_DROPFILE:
			SDL_free(e.drop.file);
			break;

		case SDL_WINDOWEVENT: {
			switch (e.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED: {
				std::pair<int, int> windowSize, frameBufferSize;
				auto* sdlWindow = SDL_GetWindowFromID(e.window.windowID);
				SDL_GetWindowSize(sdlWindow, &windowSize.first, &windowSize.second);
				SDL_GL_GetDrawableSize(sdlWindow, &frameBufferSize.first, &frameBufferSize.second);
				mEventQueue.enqueue<WindowResizedEvent>(windowSize, frameBufferSize);
				break;
			}
			default:
				break;
			}

			break;
		}
		default:;
		}
	}

	mEventQueue.update<WindowResizedEvent>();
	mEventQueue.update();
}

Window DesktopPlatformContext::createWindow(std::uint16_t width, std::uint16_t height,
											std::string_view title) const noexcept
{
	return Window{width, height, title};
}

DesktopPlatformContext::DesktopPlatformContext(DesktopPlatformContext&& c) noexcept
{
	if (c.mValid)
	{
		c.mValid = false;
	}
	else
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			mValid = false;
	}
}


Key mapKeyFromScancode(const SDL_Scancode scanCode) noexcept
{
	switch (scanCode)
	{
	case SDL_SCANCODE_LSHIFT:
		return Key::LShift;
	case SDL_SCANCODE_RSHIFT:
		return Key::RShift;
	case SDL_SCANCODE_LCTRL:
		return Key::LControl;
	case SDL_SCANCODE_RCTRL:
		return Key::RControl;
	case SDL_SCANCODE_LALT:
		return Key::LAlt;
	case SDL_SCANCODE_RALT:
		return Key::RAlt;
	case SDL_SCANCODE_APPLICATION:
		return Key::System;
	case SDL_SCANCODE_MENU:
		return Key::Menu;
	case SDL_SCANCODE_ESCAPE:
		return Key::Escape;
	case SDL_SCANCODE_SEMICOLON:
		return Key::Semicolon;
	case SDL_SCANCODE_SLASH:
		return Key::Slash;
	case SDL_SCANCODE_EQUALS:
		return Key::Equal;
	case SDL_SCANCODE_MINUS:
		return Key::Hyphen;
	case SDL_SCANCODE_LEFTBRACKET:
		return Key::LBracket;
	case SDL_SCANCODE_RIGHTBRACKET:
		return Key::RBracket;
	case SDL_SCANCODE_COMMA:
		return Key::Comma;
	case SDL_SCANCODE_PERIOD:
		return Key::Period;
	case SDL_SCANCODE_APOSTROPHE:
		return Key::Quote;
	case SDL_SCANCODE_BACKSLASH:
		return Key::Backslash;
	case SDL_SCANCODE_NONUSBACKSLASH:
		return Key::Tilde;
	case SDL_SCANCODE_SPACE:
		return Key::Space;
	case SDL_SCANCODE_RETURN:
		return Key::Enter;
	case SDL_SCANCODE_BACKSPACE:
		return Key::Backspace;
	case SDL_SCANCODE_TAB:
		return Key::Tab;
	case SDL_SCANCODE_PAGEUP:
		return Key::PageUp;
	case SDL_SCANCODE_PAGEDOWN:
		return Key::PageDown;
	case SDL_SCANCODE_END:
		return Key::End;
	case SDL_SCANCODE_HOME:
		return Key::Home;
	case SDL_SCANCODE_INSERT:
		return Key::Insert;
	case SDL_SCANCODE_DELETE:
		return Key::Delete;
	case SDL_SCANCODE_KP_PLUS:
		return Key::Add;
	case SDL_SCANCODE_KP_MINUS:
		return Key::Subtract;
	case SDL_SCANCODE_KP_MULTIPLY:
		return Key::Multiply;
	case SDL_SCANCODE_KP_DIVIDE:
		return Key::Divide;
	case SDL_SCANCODE_PAUSE:
		return Key::Pause;
	case SDL_SCANCODE_F1:
		return Key::F1;
	case SDL_SCANCODE_F2:
		return Key::F2;
	case SDL_SCANCODE_F3:
		return Key::F3;
	case SDL_SCANCODE_F4:
		return Key::F4;
	case SDL_SCANCODE_F5:
		return Key::F5;
	case SDL_SCANCODE_F6:
		return Key::F6;
	case SDL_SCANCODE_F7:
		return Key::F7;
	case SDL_SCANCODE_F8:
		return Key::F8;
	case SDL_SCANCODE_F9:
		return Key::F9;
	case SDL_SCANCODE_F10:
		return Key::F10;
	case SDL_SCANCODE_F11:
		return Key::F11;
	case SDL_SCANCODE_F12:
		return Key::F12;
	case SDL_SCANCODE_F13:
		return Key::F13;
	case SDL_SCANCODE_F14:
		return Key::F14;
	case SDL_SCANCODE_F15:
		return Key::F15;
	case SDL_SCANCODE_LEFT:
		return Key::Left;
	case SDL_SCANCODE_RIGHT:
		return Key::Right;
	case SDL_SCANCODE_UP:
		return Key::Up;
	case SDL_SCANCODE_DOWN:
		return Key::Down;
	case SDL_SCANCODE_KP_0:
		return Key::Numpad0;
	case SDL_SCANCODE_KP_1:
		return Key::Numpad1;
	case SDL_SCANCODE_KP_2:
		return Key::Numpad2;
	case SDL_SCANCODE_KP_3:
		return Key::Numpad3;
	case SDL_SCANCODE_KP_4:
		return Key::Numpad4;
	case SDL_SCANCODE_KP_5:
		return Key::Numpad5;
	case SDL_SCANCODE_KP_6:
		return Key::Numpad6;
	case SDL_SCANCODE_KP_7:
		return Key::Numpad7;
	case SDL_SCANCODE_KP_8:
		return Key::Numpad8;
	case SDL_SCANCODE_KP_9:
		return Key::Numpad9;
	case SDL_SCANCODE_A:
		return Key::A;
	case SDL_SCANCODE_B:
		return Key::B;
	case SDL_SCANCODE_C:
		return Key::C;
	case SDL_SCANCODE_D:
		return Key::D;
	case SDL_SCANCODE_E:
		return Key::E;
	case SDL_SCANCODE_F:
		return Key::F;
	case SDL_SCANCODE_G:
		return Key::G;
	case SDL_SCANCODE_H:
		return Key::H;
	case SDL_SCANCODE_I:
		return Key::I;
	case SDL_SCANCODE_J:
		return Key::J;
	case SDL_SCANCODE_K:
		return Key::K;
	case SDL_SCANCODE_L:
		return Key::L;
	case SDL_SCANCODE_M:
		return Key::M;
	case SDL_SCANCODE_N:
		return Key::N;
	case SDL_SCANCODE_O:
		return Key::O;
	case SDL_SCANCODE_P:
		return Key::P;
	case SDL_SCANCODE_Q:
		return Key::Q;
	case SDL_SCANCODE_R:
		return Key::R;
	case SDL_SCANCODE_S:
		return Key::S;
	case SDL_SCANCODE_T:
		return Key::T;
	case SDL_SCANCODE_U:
		return Key::U;
	case SDL_SCANCODE_V:
		return Key::V;
	case SDL_SCANCODE_W:
		return Key::W;
	case SDL_SCANCODE_X:
		return Key::X;
	case SDL_SCANCODE_Y:
		return Key::Y;
	case SDL_SCANCODE_Z:
		return Key::Z;
	case SDL_SCANCODE_0:
		return Key::Num0;
	case SDL_SCANCODE_1:
		return Key::Num1;
	case SDL_SCANCODE_2:
		return Key::Num2;
	case SDL_SCANCODE_3:
		return Key::Num3;
	case SDL_SCANCODE_4:
		return Key::Num4;
	case SDL_SCANCODE_5:
		return Key::Num5;
	case SDL_SCANCODE_6:
		return Key::Num6;
	case SDL_SCANCODE_7:
		return Key::Num7;
	case SDL_SCANCODE_8:
		return Key::Num8;
	case SDL_SCANCODE_9:
		return Key::Num9;
	default:
		return Key::UnknownKey;
	}
}

Key mapKeyFromMouseButton(int mouseButton) noexcept
{
	switch (mouseButton)
	{
	case SDL_BUTTON_LEFT:
		return Key::MouseLeft;
	case SDL_BUTTON_RIGHT:
		return Key::MouseRight;
	case SDL_BUTTON_MIDDLE:
		return Key::MouseMiddle;
	default:
		return Key::UnknownKey;
	}
}

} // namespace spatial
