#include "Gui.h"
#include "../../Debug.h"
#include "../../Game.h"

#include <SDL_mixer.h>

void Gui::update() noexcept
{
	button = SDL_GetMouseState(&mouse.x, &mouse.y);

	if (button != SDL_BUTTON_LMASK)
		click.x = click.y = -1;
	else if (click.x == -1 && click.y == -1)
		click = mouse;

	hoveredOver = SDL_PointInRect(&mouse, &tf);
	pressed = SDL_PointInRect(&click, &tf);

	if (pressed)
		clickTime = std::chrono::steady_clock::now().time_since_epoch();

	released = !pressed && std::chrono::steady_clock::now().time_since_epoch() - clickTime < std::chrono::milliseconds{100};

	if (released)
	{
		clickTime = std::chrono::milliseconds::zero();
		Mix_PlayChannel(7, game.audioc.getChunk(7), 0);
	}
}

Gui::Gui(SDL_Rect tf, const Game &game) noexcept
	: tf{tf},
	game{game}
{
}
