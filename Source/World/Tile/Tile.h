#pragma once

#include <SDL_render.h>
#include <SDL_rect.h>

#include <cstdint>

class World;
class Game;

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
		const int maxHealth;
		int health = maxHealth;

		Properties(std::uint32_t commonId, SDL_Point pos, bool collidables, bool breakable, int health) noexcept;
	};

	Properties props;

	virtual ~Tile() = default;

	virtual void update() noexcept;
	virtual void render(SDL_Renderer *rdr) const noexcept;

protected:
	const Game &game;

	Tile(Properties props, const World &world, const Game &game) noexcept;
	void renderTransition(SDL_Renderer *rdr, SDL_Texture *ss, SDL_Rect srcRect) const noexcept;

	const SDL_FRect *getDstRect() const noexcept { return &dstRect; };

private:
	const World &world;
	SDL_FRect dstRect;
};