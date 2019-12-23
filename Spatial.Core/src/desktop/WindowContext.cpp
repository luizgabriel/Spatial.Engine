#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>

using namespace spatial::common;

namespace spatial::desktop
{

WindowContext::WindowContext()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("Could not initialize SDL");
}

WindowContext::~WindowContext()
{
    SDL_Quit();
}

void WindowContext::pollEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {

        case SDL_QUIT:
            m_eventQueue.enqueue<desktop::WindowClosedEvent>();
            break;

        case SDL_KEYDOWN:
        {
            int scanCode = static_cast<int>(e.key.keysym.scancode);
            m_eventQueue.enqueue<desktop::KeyEvent>(scanCode, e.key.repeat, KeyAction::Pressed);

            break;
        }

        case SDL_KEYUP:
        {
            int scanCode = static_cast<int>(e.key.keysym.scancode);
            // TODO: Build spatial key enum class
            m_eventQueue.enqueue<desktop::KeyEvent>(scanCode, e.key.repeat, KeyAction::Released);

            if (scanCode == SDL_SCANCODE_ESCAPE)
            {
                m_eventQueue.enqueue<desktop::WindowClosedEvent>();
            }

            break;
        }

        case SDL_MOUSEWHEEL:
            m_eventQueue.enqueue<desktop::MouseScrolledEvent>(e.wheel.x, e.wheel.y);
            break;

        case SDL_MOUSEBUTTONDOWN:
            m_eventQueue.enqueue<desktop::MouseButtonEvent>(e.button.button, KeyAction::Pressed);
            break;

        case SDL_MOUSEBUTTONUP:
            m_eventQueue.enqueue<desktop::MouseButtonEvent>(e.button.button, KeyAction::Released);
            break;

        case SDL_MOUSEMOTION:
            m_eventQueue.enqueue<desktop::MouseMovedEvent>(e.motion.x, e.motion.y);
            break;

        case SDL_DROPFILE:
            SDL_free(e.drop.file);
            break;

        case SDL_WINDOWEVENT:
        {
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                m_eventQueue.enqueue<desktop::WindowResizedEvent>(e.window.data1, e.window.data2);
                break;
            default:
                break;
            }

            break;
        }

        default:
            break;
        }
    }

    m_eventQueue.update();
}

Window WindowContext::createWindow(int width, int height, std::string_view title)
{
    return Window{width, height, title};
}

} // namespace spatial::desktop