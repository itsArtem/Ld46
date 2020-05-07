#pragma once

#include "Tile.h"

#include <SDL_rect.h>
#include <SDL_render.h>

class World;
class Game;

class TextureTile : public Tile
{
public:
	TextureTile(Properties props, const World &world, const Game &game, SDL_Texture *ss, SDL_Rect firstSrcRect, int variations) noexcept;

	TextureTile(const TextureTile &textureTile) noexcept = default;
	TextureTile(TextureTile &&textureTile) noexcept = default;

	virtual ~TextureTile() = default;

	TextureTile &operator =(const TextureTile &textureTile) noexcept = default;
	TextureTile &operator =(TextureTile &&textureTile) noexcept = default;
	
	void render(SDL_Renderer *rdr) const noexcept final override;

private:
	const World &world;

	SDL_Texture *ss;
	SDL_Point firstSrc;
	const SDL_Rect srcRect;
};

class GrassTile final : public TextureTile
{
public:
	GrassTile(SDL_Point pos, const World &world, const Game &game) noexcept;
};

class DirtTile final : public TextureTile
{
public:
	DirtTile(SDL_Point pos, const World &world, const Game &game) noexcept;
};

class StoneTile final : public TextureTile
{
public:
	StoneTile(SDL_Point pos, const World &world, const Game &game) noexcept;
};

class WoodTile final : public TextureTile
{
public:
	WoodTile(SDL_Point pos, const World &world, const Game &game) noexcept;
};

class SnowTile final : public TextureTile
{
public:
	SnowTile(SDL_Point pos, const World &world, const Game &game) noexcept;
};

class SandTile final : public TextureTile
{
public:
	SandTile(SDL_Point pos, const World &world, const Game &game) noexcept;
};

class StoneBrickTile final : public TextureTile
{
public:
	StoneBrickTile(SDL_Point pos, const World &world, const Game &game) noexcept;
};

class BrickTile final : public TextureTile
{
public:
	BrickTile(SDL_Point pos, const World &world, const Game &game) noexcept;
};

class IronTile final : public TextureTile
{
public:
	IronTile(SDL_Point pos, const World &world, const Game &game) noexcept;
};

class ObsidianTile final : public TextureTile
{
public:
	ObsidianTile(SDL_Point pos, const World &world, const Game &game) noexcept;
};

class MetalTile final : public TextureTile
{
public:
	MetalTile(SDL_Point pos, const World &world, const Game &game) noexcept;
};