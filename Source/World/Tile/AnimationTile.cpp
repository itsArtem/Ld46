#include "AnimationTile.h"
#include "../World.h"
#include "../../Game.h"
#include "../../Random.h"

void AnimationTile::update() noexcept
{
	Tile::update();
	animation.update(game.getDelta());
}

void AnimationTile::render(SDL_Renderer *rdr) const noexcept
{
	animation.render(rdr, *getDstRect());
	renderTransition(rdr, game.texc.get(0), {256, 0, 16, 16});
	Tile::render(rdr);
}


AnimationTile::AnimationTile(Properties props, const World &world, const Animation &animation, const Game &game) noexcept
	: Tile{props, world, game},
	world{world},
	animation{animation}
{
}

WaterTile::WaterTile(SDL_Point pos, const World &world, const Game &game) noexcept
	: AnimationTile{{11, pos, true, false, 0}, world, {game.texc.get(0), {128, 240, 16, 16}, 3, getRandomReal(150.0f, 200.0f)}, game}
{
}
