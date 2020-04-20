#pragma once

#include "Tile/Tile.h"
#include "../Game.h"
#include "Ecs/EntityManager.h"

#include <SDL_render.h>
#include <SDL_rect.h>

#include <vector>
#include <memory>
#include <functional>
#include <thread>

class World final
{
public:
	SDL_Point size{20, 20};
	SDL_FPoint camera{0.0f, 0.0f};
	
	EntityManager em;
	//int darkness = 0;

	const int finalWave = 10;

	explicit World(Game &game) noexcept;

	void update() noexcept;
	void render() const noexcept;
	
	//void emitLight(SDL_Point pos, int radius, int darkness) noexcept;

	[[nodiscard]] std::unique_ptr<Tile> &getTile(SDL_Point pos) noexcept;
	[[nodiscard]] const std::unique_ptr<Tile> &getTile(SDL_Point pos) const noexcept;
	[[nodiscard]] bool isInside(SDL_Point pos) const noexcept;

	[[nodiscard]] std::uint32_t getPlayer() const noexcept { return player; }
	[[nodiscard]] std::uint32_t getChest() const noexcept { return chest; }
	[[nodiscard]] int getWave() const noexcept { return wave; }
	[[nodiscard]] int getLevel() const noexcept { return level; }

private:
	Game &game;

	std::vector<std::unique_ptr<Tile>> tiles;
	std::vector<int> light;
	std::uint32_t player = 0;
	std::uint32_t chest = 0;
	
	//SDL_Texture *const lightSheet = game.texc.get(0);
	//SDL_Rect firstLightSrc{240, 208, 16, 16};

	//const int maxDarkness = 5;
	//const float lightFadeDelay = 150.0f;
	//float lightFadeTimer = lightFadeDelay;

	int level = 1;
	const int finalLevel = 10;
	int wave = 0;
	bool canBuild = false;

	SDL_Point mouse;
	std::unordered_map<int, int> costs;
	SDL_Rect buildBounds{64, 64, game.getWindowSize().x - 64, game.getWindowSize().y - 64};
	SDL_Rect outlineDstRect{0, 0, Tile::Properties::size, Tile::Properties::size};
	SDL_Rect outlineSrcRect{128, 0, 16, 16};

	//void createLightRadius(SDL_Point pos, int radius, int darkness) noexcept;

	void forAllVisibleTiles(std::function<void(Tile &tile)> func) const noexcept;
	//void forAllVisibleLight(std::function<void(int &light)> func) noexcept;
	[[nodiscard]] SDL_Rect getView() const noexcept;

	void load(const std::string &path);
	void progress() noexcept;
	void spawnRandomHostile(SDL_FPoint pos) noexcept;
};