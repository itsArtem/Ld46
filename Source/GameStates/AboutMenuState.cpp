#include "AboutMenuState.h"
#include "../Game.h"
#include "../Graphics/Gui/TextButton.h"
#include "GameplayState.h"

#include <SDL_render.h>

AboutMenuState::AboutMenuState(Game &game)
	: game{game},
	title{game.fontc.get(2), "About", {255, 255, 100, 255}, true, game.getRenderer()}
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	title.dstRect.y = 50;

	info.setWrappedText(game.fontc.get(1),
		"\"Defend the Loot!\" is a game\n"
		"made from scratch in 48 hours\n"
		"by Artem Kateryncy\n" 
		"for Ludum Dare 46.\n"
		"\n"
		"Thanks for playing!\n"
		"April 19, 2020",
		{255, 255, 255, 255},
		480,
		game.getRenderer());

	info.dstRect.w = 470.0f;
	info.dstRect.h = 210.0f;
	info.dstRect.x = game.getWindowSize().x - 600.0f;
	info.dstRect.y = 220.0f;

	canvas.add(std::make_unique<TextButton>(SDL_Point{200, 295}, game, TextButton::Properties{Text{game.fontc.get(1), "Back", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
}

void AboutMenuState::update() noexcept
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	info.dstRect.x = game.getWindowSize().x - 600.0f;
	canvas.update();

	if (canvas.get(0).isReleased())
		game.gsm.pop();
}

void AboutMenuState::render() const noexcept
{
	SDL_Renderer *const rdr = game.getRenderer();

	title.render(rdr, Text::Shadow::none);
	info.render(rdr, Text::Shadow::none);
	canvas.render(rdr);
}
