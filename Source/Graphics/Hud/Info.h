#pragma once

#include "../../Game.h"
#include "../../Graphics/Text.h"

#include <SDL_render.h>
#include <SDL_rect.h>

class World;

class Info final
{
public:
	Info(const Game &game, const World &world) noexcept;

	void update() noexcept;
	void render() const noexcept;

private:
	const Game &game;
	const World &world;

	Text prep;
	Text points;
	Text wave, level;

	SDL_Texture *bg = game.texc.get(2);
	SDL_Rect lbgSrcRect{0, 32, 80, 48};
	SDL_Rect rbgSrcRect{0, 80, 80, 32};
};