#include "Tile.h"
#include "../World.h"
#include "../../Game.h"

void Tile::update() noexcept
{
	dstRect.x = props.pos.x * Properties::size - world.camera.x;
	dstRect.y = props.pos.y * Properties::size - world.camera.y;
}

void Tile::render(SDL_Renderer *rdr) const noexcept
{
	if (!props.collidable || !props.breakable)
		return;

	const float precentage = static_cast<float>(props.health) / static_cast<float>(props.maxHealth) * 100;
	SDL_Texture *ss = game.texc.get(0);
	SDL_Rect srcRect;

	if (precentage <= 10.0f)
		srcRect = {336, 272, 16, 16};
	if (precentage <= 20.0f)
		srcRect = {336, 256, 16, 16};
	else if (precentage <= 30.0f)
		srcRect = {336, 240, 16, 16};
	else if (precentage <= 45.0f)
		srcRect = {336, 224, 16, 16};
	else if (precentage <= 60.0f)
		srcRect = {336, 208, 16, 16};
	else if (precentage <= 75.0f)
		srcRect = {336, 192, 16, 16};
	else if (precentage <= 90.0f)
		srcRect = {336, 176, 16, 16};
	else
		return;

	SDL_RenderCopyF(rdr, ss, &srcRect, getDstRect());
}


Tile::Tile(Properties props, const World &world, const Game &game) noexcept
	: props{props},
	world{world},
	game{game},
	dstRect{props.pos.x * Properties::size - world.camera.x, props.pos.y * Properties::size - world.camera.y, Properties::size, Properties::size}
{
}

void Tile::renderTransition(SDL_Renderer *rdr, SDL_Texture *ss, SDL_Rect srcRect) const noexcept
{
	std::function<bool(SDL_Point)> isConnected = [&world = world, props = props](SDL_Point pos)
	{
		if (!world.isInside(pos))
			return true;

		return world.getTile(pos)->props.commonId == props.commonId;
	};

	const bool u = !isConnected({props.pos.x, props.pos.y - 1});
	const bool d = !isConnected({props.pos.x, props.pos.y + 1});
	const bool l = !isConnected({props.pos.x - 1, props.pos.y});
	const bool r = !isConnected({props.pos.x + 1, props.pos.y});

	if (u && d && l && r)
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w * 4, srcRect.y + srcRect.h * 2, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u && l && r)
	{
		const SDL_Rect transSrcRect{srcRect.x, srcRect.y + srcRect.h * 2, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (d && l && r)
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w, srcRect.y + srcRect.h * 2, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u && d && l)
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w * 2, srcRect.y + srcRect.h * 2, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u && d && r)
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w * 3, srcRect.y + srcRect.h * 2, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u && l)
	{
		const SDL_Rect transSrcRect{srcRect.x, srcRect.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u && r)
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w, srcRect.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (d && l)
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w * 2, srcRect.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (d && r)
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w * 3, srcRect.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u && d)
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w * 4, srcRect.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (l && r)
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w * 4, srcRect.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u)
	{
		const SDL_Rect transSrcRect{srcRect.x, srcRect.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (d)
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w, srcRect.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (l)
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w * 2, srcRect.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (r)
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w * 3, srcRect.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}

	const bool dr = !isConnected({props.pos.x + 1, props.pos.y + 1});
	const bool dl = !isConnected({props.pos.x - 1, props.pos.y + 1});
	const bool ur = !isConnected({props.pos.x + 1, props.pos.y - 1});
	const bool ul = !isConnected({props.pos.x - 1, props.pos.y - 1});

	const bool free = !u && !d && !l && !r;

	if (dr && (free || !d && !r))
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w * 5, srcRect.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}

	if (dl && (free || !d && !l))
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w * 6, srcRect.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}

	if (ur && (free || !u && !r))
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w * 5, srcRect.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}

	if (ul && (free || !u && !l))
	{
		const SDL_Rect transSrcRect{srcRect.x + srcRect.w * 6, srcRect.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
}

Tile::Properties::Properties(std::uint32_t commonId, SDL_Point pos, bool collidables, bool breakable, int health) noexcept
	: commonId{commonId},
	pos{pos},
	collidable{collidables},
	breakable{breakable},
	maxHealth{health}
{
}