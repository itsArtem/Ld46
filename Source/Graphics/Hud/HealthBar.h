#pragma once

#include "../../Game.h"
#include <SDL_render.h>

class Entity;
class World;

class HealthBar final
{
public:
	explicit HealthBar(World &world, const Game &game) noexcept;

	void update() noexcept;
	void render() const noexcept;

private:
	World &world;
	const Game &game;

	SDL_Texture *ss = game.texc.get(2);
	const SDL_Rect iconSrc{96, 0, 16, 16};
	const SDL_Rect barSrc{48, 0, 48, 8};
	const SDL_Rect emptyBarSrc{48, 8, 48, 8};
	const SDL_Rect iconDst{8, 8, 48, 48};
	const SDL_Rect barDst{iconDst.x + 28, iconDst.y + 4, 256, 38};
	SDL_Rect healthMetre{barDst.x + 11, barDst.y + 6, 0, barDst.h - 12};

	const SDL_Rect chestIconSrc{112, 0, 16, 16};
	SDL_Rect chestIconDst{game.getWindowSize().x - 56, 8, 48, 48};
	SDL_Rect chestBarDst{chestIconDst.x - chestBarDst.w, iconDst.y + 4, 256, 38};
	SDL_Rect chestHealthMetre{chestBarDst.x, barDst.y + 6, 0, barDst.h - 12};
};