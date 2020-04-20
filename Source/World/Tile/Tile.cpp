#include "Tile.h"
#include "../World.h"

Tile::Tile(Properties props, const World &world) noexcept
	: props{props},
	world{world},
	dstRect{props.pos.x * Properties::size - world.camera.x, props.pos.y * Properties::size - world.camera.y, Properties::size, Properties::size}
{
}

Tile::~Tile() = default;

void Tile::update() noexcept
{
	dstRect.x = props.pos.x * Properties::size - world.camera.x;
	dstRect.y = props.pos.y * Properties::size - world.camera.y;
}

Tile::Properties::Properties(std::uint32_t commonId, SDL_Point pos, bool collidables, bool breakable, int health) noexcept
	: commonId{commonId},
	pos{pos},
	collidable{collidables},
	breakable{breakable},
	health{health}
{
}