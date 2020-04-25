#include "LevelPassMenuState.h"
#include "../Game.h"
#include "../Graphics/Gui/TextButton.h"
#include "GameplayState.h"

#include <SDL_render.h>

LevelPassMenuState::LevelPassMenuState(Game &game, int score)
	: game{game},
	title{game.fontc.get(2), "Level Complete", {255, 255, 100, 255}, true, game.getRenderer()}
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	title.dstRect.y = 50;

	info.setWrappedText(game.fontc.get(1),
		"You passed the level!\n"
		"Score: " + std::to_string(score),
		{255, 255, 255, 255},
		480,
		game.getRenderer());

	info.dstRect.w = 460.0f;
	info.dstRect.h = 64.0f;
	info.dstRect.x = game.getWindowSize().x - 600.0f;
	info.dstRect.y = 220.0f;

	canvas.add(std::make_unique<TextButton>(SDL_Point{200, 245}, game, TextButton::Properties{Text{game.fontc.get(1), "Next Level", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
}

void LevelPassMenuState::update() noexcept
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	info.dstRect.x = game.getWindowSize().x - 600.0f;
	canvas.update();

	if (canvas.get(0).isReleased())
		game.gsm.pop();
}

void LevelPassMenuState::render() const noexcept
{
	SDL_Renderer *const rdr = game.getRenderer();

	title.render(rdr, Text::Shadow::none);
	info.render(rdr, Text::Shadow::none);
	canvas.render(rdr);
}
