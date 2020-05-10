#include "MainMenuState.h"
#include "../Game.h"
#include "../Graphics/Gui/TextButton.h"
#include "GameplayState.h"
#include "AboutMenuState.h"
#include "OptionsMenuState.h"

#include <SDL_render.h>

#include <string>

MainMenuState::MainMenuState(Game &game)
	: game{game},
	title{game.fontc.get(2), "Defend the Loot!", {255, 255, 100, 255}, true, game.getRenderer()}
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	title.dstRect.y = 50;

	info.setWrappedText(game.fontc.get(1),
		"Move around with WASD.\n"
		"Left click to buy block and right click to sell.\n"
		"Defend your loot from waves of enemies.\n"
		"Get to the final level to win!",
		{255, 255, 255, 255},
		480,
		game.getRenderer());

	info.dstRect.w = 480.0f;
	info.dstRect.h = 180.0f;
	info.dstRect.x = game.getWindowSize().x - 600.0f;
	info.dstRect.y = 220.0f;

	version.setText(game.fontc.get(0), "Version 2.2.0", {100, 100, 100, 255}, true, game.getRenderer());
	version.dstRect.x = 4;
	version.dstRect.y = game.getWindowSize().y - version.dstRect.h - 4;

	if (!game.gameSave.doesFileExist())
	{
		canvas.add(std::make_unique<TextButton>(SDL_Point{200, 216}, game, TextButton::Properties{Text{game.fontc.get(1), "New Game", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
		canvas.add(std::make_unique<TextButton>(SDL_Point{200, 266}, game, TextButton::Properties{Text{game.fontc.get(1), "Options", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
		canvas.add(std::make_unique<TextButton>(SDL_Point{200, 316}, game, TextButton::Properties{Text{game.fontc.get(1), "About", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
		canvas.add(std::make_unique<TextButton>(SDL_Point{200, 366}, game, TextButton::Properties{Text{game.fontc.get(1), "Quit", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
	}
	else
	{
		canvas.add(std::make_unique<TextButton>(SDL_Point{200, 194}, game, TextButton::Properties{Text{game.fontc.get(1), "Continue", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
		canvas.add(std::make_unique<TextButton>(SDL_Point{200, 244}, game, TextButton::Properties{Text{game.fontc.get(1), "New Game", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
		canvas.add(std::make_unique<TextButton>(SDL_Point{200, 294}, game, TextButton::Properties{Text{game.fontc.get(1), "Options", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
		canvas.add(std::make_unique<TextButton>(SDL_Point{200, 344}, game, TextButton::Properties{Text{game.fontc.get(1), "About", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
		canvas.add(std::make_unique<TextButton>(SDL_Point{200, 394}, game, TextButton::Properties{Text{game.fontc.get(1), "Quit", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
	}
}

void MainMenuState::update() noexcept
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	info.dstRect.x = game.getWindowSize().x - 600.0f;
	version.dstRect.y = game.getWindowSize().y - version.dstRect.h - 4;
	canvas.update();

	if (!game.gameSave.doesFileExist())
	{
		if (canvas.get(0).isReleased())
		{
			game.gameSave.save();
			game.gsm.add(std::make_unique<GameplayState>(game));
			game.gsm.remove(game.gsm.getCount() - 2);
			return;
		}

		if (canvas.get(1).isReleased())
			game.gsm.add(std::make_unique<OptionsMenuState>(game));

		if (canvas.get(2).isReleased())
			game.gsm.add(std::make_unique<AboutMenuState>(game));

		if (canvas.get(3).isReleased())
			game.running = false;
	}
	else
	{
		if (canvas.get(0).isReleased() || canvas.get(1).isReleased())
		{
			if (canvas.get(0).isReleased())
				game.gameSave.load();
			else
			{
				game.gameSave.reset();
				game.gameSave.save();
			}

			game.gsm.add(std::make_unique<GameplayState>(game));
			game.gsm.remove(game.gsm.getCount() - 2);
			return;
		}

		if (canvas.get(2).isReleased())
			game.gsm.add(std::make_unique<OptionsMenuState>(game));

		if (canvas.get(3).isReleased())
			game.gsm.add(std::make_unique<AboutMenuState>(game));

		if (canvas.get(4).isReleased())
			game.running = false;
	}
}

void MainMenuState::render() const noexcept
{
	SDL_Renderer *const rdr = game.getRenderer();

	title.render(rdr, Text::Shadow::none);
	info.render(rdr, Text::Shadow::none);
	version.render(rdr, Text::Shadow::none);
	canvas.render(rdr);
}
