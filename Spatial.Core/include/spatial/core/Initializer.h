#pragma once

#include <spatial/core/Application.h>
namespace spatial
{

template <typename Functor>
auto tryRun(const Functor& action)
{
	try
	{
		return action();
	}
	catch (const std::runtime_error& e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "[Spatial Engine] Something went wrong", e.what(), nullptr);
		return -1;
	}
}

}
