#include "TextureTile.h"
#include "../World.h"
#include "../../Game.h"

#include <SDL_render.h>

#include <functional>

void TextureTile::render(SDL_Renderer *rdr) const noexcept
{
	SDL_SetTextureColorMod(ss, 255, 255, 255);
	SDL_RenderCopyF(rdr, ss, &srcRect, getDstRect());
	renderTransition(rdr, ss, {firstSrc.x + 16, firstSrc.y, srcRect.w, srcRect.h});
	Tile::render(rdr);
	/*
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
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 5, firstSrc.y + srcRect.h * 2, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u && l && r)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w, firstSrc.y + srcRect.h * 2, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (d && l && r)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 2, firstSrc.y + srcRect.h * 2, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u && d && l)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 3, firstSrc.y + srcRect.h * 2, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u && d && r)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 4, firstSrc.y + srcRect.h * 2, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u && l)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w, firstSrc.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u && r)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 2, firstSrc.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (d && l)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 3, firstSrc.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (d && r)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 4, firstSrc.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u && d)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 5, firstSrc.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (l && r)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 5, firstSrc.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (u)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w, firstSrc.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (d)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 2, firstSrc.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (l)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 3, firstSrc.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	else if (r)
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 4, firstSrc.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}

	const bool dr = !isConnected({props.pos.x + 1, props.pos.y + 1});
	const bool dl = !isConnected({props.pos.x - 1, props.pos.y + 1});
	const bool ur = !isConnected({props.pos.x + 1, props.pos.y - 1});
	const bool ul = !isConnected({props.pos.x - 1, props.pos.y - 1});

	const bool free = !u && !d && !l && !r;

	if (dr && (free || !d && !r))
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 6, firstSrc.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}

	if (dl && (free || !d && !l))
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 7, firstSrc.y, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}

	if (ur && (free || !u && !r))
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 6, firstSrc.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}

	if (ul && (free || !u && !l))
	{
		const SDL_Rect transSrcRect{firstSrc.x + srcRect.w * 7, firstSrc.y + srcRect.h, srcRect.w, srcRect.h};
		SDL_RenderCopyF(rdr, ss, &transSrcRect, getDstRect());
	}
	*/
}

TextureTile::TextureTile(Properties props, const World &world, const Game &game, SDL_Texture *ss, SDL_Rect firstSrcRect, int variations) noexcept
	: Tile{props, world, game},
	world{world},
	ss{ss},
	firstSrc{firstSrcRect.x, firstSrcRect.y},
	srcRect{firstSrcRect.x, firstSrcRect.y + firstSrcRect.h * ((props.pos.x + props.pos.y) % variations), firstSrcRect.w, firstSrcRect.h}
{
}

GrassTile::GrassTile(SDL_Point pos, const World &world, const Game &game) noexcept
	: TextureTile{{0, pos, false, true, 0}, world, game, game.texc.get(0), {0, 0, 16, 16}, 3}
{
}

DirtTile::DirtTile(SDL_Point pos, const World &world, const Game &game) noexcept
	: TextureTile{{1, pos, false, true, 0}, world, game, game.texc.get(0), {128, 0, 16, 16}, 3}
{
}

StoneTile::StoneTile(SDL_Point pos, const World &world, const Game &game) noexcept
	: TextureTile{{2, pos, true, false, 0}, world, game, game.texc.get(0), {0, 48, 16, 16}, 3}
{
}

WoodTile::WoodTile(SDL_Point pos, const World &world, const Game &game) noexcept
	: TextureTile{{3, pos, true, true, 150}, world, game, game.texc.get(0), {128, 48, 16, 16}, 1}
{
}

SnowTile::SnowTile(SDL_Point pos, const World &world, const Game &game) noexcept
	: TextureTile{{4, pos, false, true, 0}, world, game, game.texc.get(0), {0, 144, 16, 16}, 3}
{
}

SandTile::SandTile(SDL_Point pos, const World &world, const Game &game) noexcept
	: TextureTile{{5, pos, false, true, 0}, world, game, game.texc.get(0), {128, 144, 16, 16}, 3}
{
}

StoneBrickTile::StoneBrickTile(SDL_Point pos, const World &world, const Game &game) noexcept
	: TextureTile{{6, pos, true, true, 200}, world, game, game.texc.get(0), {0, 192, 16, 16}, 1}
{
}

BrickTile::BrickTile(SDL_Point pos, const World &world, const Game &game) noexcept
	: TextureTile{{7, pos, true, true, 250}, world, game, game.texc.get(0), {0, 96, 16, 16}, 1}
{
}

IronTile::IronTile(SDL_Point pos, const World &world, const Game &game) noexcept
	: TextureTile{{8, pos, true, true, 300}, world, game, game.texc.get(0), {128, 96, 16, 16}, 1}
{
}

ObsidianTile::ObsidianTile(SDL_Point pos, const World &world, const Game &game) noexcept
	: TextureTile{{9, pos, true, true, 450}, world, game, game.texc.get(0), {0, 240, 16, 16}, 1}
{
}

MetalTile::MetalTile(SDL_Point pos, const World &world, const Game &game) noexcept
	: TextureTile{{10, pos, true, true, 250}, world, game, game.texc.get(0), {128, 192, 16, 16}, 3}
{
}
