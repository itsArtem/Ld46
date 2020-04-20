#include "GameOverMenuState.h"
#include "../Game.h"
#include "../Graphics/Gui/TextButton.h"
#include "GameplayState.h"
#include "AboutMenuState.h"
#include "MainMenuState.h"

#include <SDL_render.h>

#include <string>

GameOverMenuState::GameOverMenuState(Game &game, int finalScore)
	: game{game},
	title{game.fontc.get(2), "Game Over", {255, 255, 100, 255}, true, game.getRenderer()}
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	title.dstRect.y = 50;

	info.setWrappedText(game.fontc.get(1),
		"F! You Lost!\n"
		"Awww!\n"
		"\n"
		"Final score: " + std::to_string(finalScore),
		{255, 255, 255, 255},
		480,
		game.getRenderer());

	info.dstRect.w = 520;
	info.dstRect.h = 120;
	info.dstRect.x = game.getWindowSize().x - 600;
	info.dstRect.y = 245;

	canvas.add(std::make_unique<TextButton>(SDL_Point{200, 280}, game, TextButton::Properties{Text{game.fontc.get(1), "Main Menu", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
}

void GameOverMenuState::update() noexcept
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	info.dstRect.x = game.getWindowSize().x - 600;
	canvas.update();

	if (canvas.get(0).isReleased())
	{
		game.toMainMenu();
		return;
	}
}

void GameOverMenuState::render() const noexcept
{
	SDL_Renderer *const rdr = game.getRenderer();

	title.render(rdr, Text::Shadow::none);
	info.render(rdr, Text::Shadow::none);
	canvas.render(rdr);
}
