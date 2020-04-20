#include "MainMenuState.h"
#include "../Game.h"
#include "../Graphics/Gui/TextButton.h"
#include "GameplayState.h"
#include "AboutMenuState.h"

#include <SDL_render.h>

MainMenuState::MainMenuState(Game &game)
	: game{game},
	title{game.fontc.get(2), "Defend the Loot!", {255, 255, 100, 255}, true, game.getRenderer()}
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	title.dstRect.y = 50;

	info.setWrappedText(game.fontc.get(1),
		"Move around with WASP.\n"
		"Left click to buy block and right click to sell.\n"
		"Defend your loot from waves of enemies.\n"
		"Get to the final level to win!",
		{255, 255, 255, 255}, 
		480,
		game.getRenderer());

	info.dstRect.w = 480;
	info.dstRect.h = 180;
	info.dstRect.x = game.getWindowSize().x - 600;
	info.dstRect.y = 220;

	canvas.add(std::make_unique<TextButton>(SDL_Point{200, 235}, game, TextButton::Properties{Text{game.fontc.get(1), "Start Game", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
	canvas.add(std::make_unique<TextButton>(SDL_Point{200, 285}, game, TextButton::Properties{Text{game.fontc.get(1), "About", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
	canvas.add(std::make_unique<TextButton>(SDL_Point{200, 335}, game, TextButton::Properties{Text{game.fontc.get(1), "Quit", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
}

void MainMenuState::update() noexcept
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	info.dstRect.x = game.getWindowSize().x - 600;
	canvas.update();

	if (canvas.get(0).isReleased())
	{
		game.gsm.add(std::make_unique<GameplayState>(game));
		game.gsm.remove(game.gsm.getCount() - 2);
		return;
	}

	if (canvas.get(1).isReleased())
	{
		game.gsm.add(std::make_unique<AboutMenuState>(game));
	}

	if (canvas.get(2).isReleased())
		game.running = false;
}

void MainMenuState::render() const noexcept
{
	SDL_Renderer *const rdr = game.getRenderer();

	title.render(rdr, Text::Shadow::none);
	info.render(rdr, Text::Shadow::none);
	canvas.render(rdr);
}
