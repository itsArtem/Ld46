#include "PauseMenuState.h"
#include "../Game.h"
#include "../Graphics/Gui/TextButton.h"

#include <SDL_render.h>

PauseMenuState::PauseMenuState(Game &game)
	: game{game},
	title{game.fontc.get(2), "Paused", {255, 255, 100, 255}, true, game.getRenderer()}
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	title.dstRect.y = 50;

	info.setWrappedText(game.fontc.get(1),
		"Move around with WASD.\n"
		"Buy block by left-clicking, and sell blocks by right-clicking.\n"
		"Click the shop icon to select a different block.\n"
		"Blocks cost points. Gain points by damaging enemies.",
		{255, 255, 255, 255},
		480,
		game.getRenderer());

	info.dstRect.w = 480.0f;
	info.dstRect.h = 210.0f;
	info.dstRect.x = game.getWindowSize().x - 600.0f;
	info.dstRect.y = 220.0f;

	canvas.add(std::make_unique<TextButton>(SDL_Point{200, 275}, game, TextButton::Properties{Text{game.fontc.get(1), "Continue", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
	canvas.add(std::make_unique<TextButton>(SDL_Point{200, 325}, game, TextButton::Properties{Text{game.fontc.get(1), "Main Menu", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
}

void PauseMenuState::update() noexcept
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	info.dstRect.x = game.getWindowSize().x - 600.0f;
	canvas.update();

	if (canvas.get(0).isReleased())
	{
		game.gsm.pop();
		return;
	}

	if (canvas.get(1).isReleased())
		game.toMainMenu();
}

void PauseMenuState::render() const noexcept
{
	SDL_Renderer *const rdr = game.getRenderer();

	title.render(rdr, Text::Shadow::none);
	info.render(rdr, Text::Shadow::right);
	canvas.render(rdr);
}
