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
            int scanCode = event.key.keysym.scancode;
            m_eventQueue.enqueue<desktop::KeyEvent>(scanCode, KeyAction::Pressed);

            if (scanCode == SDL_SCANCODE_ESCAPE)
                m_eventQueue.enqueue<desktop::WindowClosedEvent>();

            break;

        case SDL_MOUSEWHEEL:
            m_eventQueue.enqueue<desktop::MouseScrolledEvent>(event.wheel.x, event.wheel.y);
            break;

        case SDL_MOUSEBUTTONDOWN:
            m_eventQueue.enqueue<desktop::MouseButtonEvent>(event.button.button, KeyAction::Pressed);
            break;

        case SDL_MOUSEBUTTONUP:
            m_eventQueue.enqueue<desktop::MouseButtonEvent>(event.button.button, KeyAction::Released);
            break;

        case SDL_MOUSEMOTION:
            m_eventQueue.enqueue<desktop::MouseButtonEvent>(event.motion.x, event.motion.y);
            break;

        case SDL_DROPFILE:
            //
            SDL_free(event.drop.file);
            break;

        case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                m_eventQueue.enqueue<desktop::MouseButtonEvent>(event.window.width, event.window.height);
                break;
            default:
                break;
            }
            break;

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