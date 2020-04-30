#include "Game.h"
#include "Debug.h"

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_log.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <exception>

#include <vector>

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1
		|| IMG_Init(IMG_INIT_PNG) == 0
		|| Mix_Init(MIX_INIT_MID) == 0
		|| Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1
		|| TTF_Init() == -1)
	{
		LOG_CRITICAL(SDL_LOG_CATEGORY_ERROR, SDL_GetError())
		return 1;
	}

#if DEBUG
	try
	{
		Game{}.run();
	}
	catch (std::exception &e)
	{
		LOG_CRITICAL(SDL_LOG_CATEGORY_ERROR, e.what())
		return 1;
	}
#else
	{
		Game{}.run();
	}
#endif

	SDL_Quit();
	IMG_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();

	return 0;
}