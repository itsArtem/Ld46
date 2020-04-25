#include "WinMenuState.h"
#include "../Game.h"
#include "../Graphics/Gui/TextButton.h"
#include "GameplayState.h"
#include "AboutMenuState.h"
#include "MainMenuState.h"

#include <SDL_render.h>

#include <string>

WinMenuState::WinMenuState(Game &game, int finalScore)
	: game{game},
	world{world},
	title{game.fontc.get(2), "You Won!", {255, 255, 100, 255}, true, game.getRenderer()}
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	title.dstRect.y = 50;

	info.setWrappedText(game.fontc.get(1),
		"Yay! You Won!\n"
		"Final score: " + std::to_string(finalScore) + "\n"
		"\n"
		"Thank you for playing!",
		{255, 255, 255, 255}, 
		480,
		game.getRenderer());

	info.dstRect.w = 500.0f;
	info.dstRect.h = 120.0f;
	info.dstRect.x = game.getWindowSize().x - 600.0f;
	info.dstRect.y = 220.0f;

	canvas.add(std::make_unique<TextButton>(SDL_Point{200, 255}, game, TextButton::Properties{Text{game.fontc.get(1), "Main Menu", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
}

void WinMenuState::update() noexcept
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	info.dstRect.x = game.getWindowSize().x - 600.0f;
	canvas.update();

	if (canvas.get(0).isReleased())
	{
		game.toMainMenu();
		return;
	}
}

void WinMenuState::render() const noexcept
{
	SDL_Renderer *const rdr = game.getRenderer();

	title.render(rdr, Text::Shadow::none);
	info.render(rdr, Text::Shadow::none);
	canvas.render(rdr);
}
