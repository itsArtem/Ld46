#pragma once

#include "Tile.h"
#include "../../Graphics/Animation.h"

#include <SDL_render.h>
#include <SDL_rect.h>

class World;
class Game;

class AnimationTile : public Tile
{
public:
	void update() noexcept final override;
	void render(SDL_Renderer *rdr) const noexcept final override;

protected:
	AnimationTile(Properties props, const World &world, const Animation &animation, const Game &game) noexcept;

	AnimationTile(const AnimationTile &animationTile) = default;
	AnimationTile(AnimationTile && animationTile) = default;

	virtual ~AnimationTile() = default;

	AnimationTile &operator =(const AnimationTile & animationTile) = default;
	AnimationTile &operator =(AnimationTile && animationTile) = default;

private:
	const World &world;
	Animation animation;
};

class WaterTile final : public AnimationTile
{
public:
	WaterTile(SDL_Point pos, const World &world, const Game &game) noexcept;
};