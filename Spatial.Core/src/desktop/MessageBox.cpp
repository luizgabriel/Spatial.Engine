#include <spatial/desktop/MessageBox.h>
#include <SDL.h>

namespace spatial
{

void showMessageBox(MessageBoxType type, std::string_view title, std::string_view message)
{
	uint32_t flags = 0;
	switch (type)
	{
	case MessageBoxType::Info: flags |= SDL_MESSAGEBOX_INFORMATION; break;
	case MessageBoxType::Warning: flags |= SDL_MESSAGEBOX_WARNING; break;
	case MessageBoxType::Error: flags |= SDL_MESSAGEBOX_ERROR; break;
	}

	SDL_ShowSimpleMessageBox(flags, title.data(), message.data(), nullptr);
}

} // namespace spatial