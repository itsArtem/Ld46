#include "AnimationTile.h"
#include "../World.h"
#include "../../Game.h"

#include <SDL_render.h>

#include <functional>

AnimationTile::AnimationTile(Properties props, const World &world, const Animation &animation, const Game &game) noexcept
	: Tile{props, world},
	world{world},
	animation{animation},
	game{game}
{
}

void AnimationTile::update() noexcept
{
	animation.update(game.getDelta());
}

void AnimationTile::render(SDL_Renderer *rdr) const noexcept
{
	animation.render(rdr, *getDstRect());
}