#pragma once

#include <SDL_render.h>
#include <SDL_rect.h>

#include <cstdint>

class World;

class Tile
{
public:
	struct Properties final
	{
		const std::uint32_t commonId;

		const SDL_Point pos;
		static constexpr int size = 64;

		const bool collidable;
		const bool breakable;
		int health;

		Properties(std::uint32_t commonId, SDL_Point pos, bool collidables, bool breakable, int health) noexcept;
	};

	Properties props;

	Tile(Properties props, const World &world) noexcept;
	virtual ~Tile();

	virtual void update() noexcept;
	virtual void render(SDL_Renderer *rdr) const noexcept = 0;

protected:
	const SDL_FRect *getDstRect() const noexcept { return &dstRect; };

private:
	const World &world;
	SDL_FRect dstRect;
};