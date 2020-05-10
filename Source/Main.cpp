#include "Game.h"
#include "Debug.h"

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_log.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <exception>
#include <string>

static int showCrashMessage(const std::string &crashMessage) 
{
	const SDL_MessageBoxButtonData button{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Aww"};
	const std::string message{"Looks like the game has crashed, that shouldn't have happened.\nYou should report this error on the Itch.io or Ludum Dare page.\n\nHere's what went down:\n" + crashMessage};
	const SDL_MessageBoxData data{SDL_MESSAGEBOX_ERROR, nullptr, "CRASH!", message.c_str(), 1, &button, nullptr};

	int buttonPressed;
	SDL_ShowMessageBox(&data, &buttonPressed);

	return buttonPressed;
}

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1
		|| IMG_Init(IMG_INIT_PNG) == 0
		|| Mix_Init(MIX_INIT_MID) == 0
		|| Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1
		|| TTF_Init() == -1) [[unlikely]]
	{
		LOG_CRITICAL(SDL_LOG_CATEGORY_ERROR, SDL_GetError())
		showCrashMessage(SDL_GetError());
		return 1;
	}
	
	int exitCode = 0;

	try
	{
		Game{}.run();
	}
	catch (std::exception &e)
	{
		LOG_CRITICAL(SDL_LOG_CATEGORY_ERROR, e.what())
		showCrashMessage(e.what());
		exitCode = 1;
	}
	
	SDL_Quit();
	IMG_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();

	return exitCode;
}