#include "World.h"
#include "Tile/TextureTile.h"
#include "Tile/AnimationTile.h"
#include "../Debug.h"
#include "Ecs/Component.h"
#include "../Random.h"
#include "../GameStates/WinMenuState.h"
#include "../GameStates/GameOverMenuState.h"
#include "../GameStates/LevelPassMenuState.h"

#include <SDL_surface.h>
#include <SDL_pixels.h>
#include <SDL_log.h>
#include <SDL_mouse.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <memory>
#include <stdexcept>
#include <fstream>
#include <cstdint>

World::World(Game &game) noexcept
	: game{game}
{
	load("Resources/Levels/Level" + std::to_string(game.gameSave.level) + ".png");

	costs.emplace(3, 60);
	costs.emplace(6, 140);
	costs.emplace(7, 200);
	costs.emplace(8, 350);
	costs.emplace(9, 600);
	costs.emplace(10, 260);
}

void World::update() noexcept
{
	const float delta = game.getDelta();
	//lightFadeTimer -= 1000 * delta;

	//if (lightFadeTimer <= 0.0f)
	//{
	//	forAllVisibleLight([darkness = darkness](int &light)
	//		{
	//			if (light < darkness)
	//				++light;
	//		});

	//	lightFadeTimer = lightFadeDelay;
	//}

	forAllVisibleTiles([this](Tile &tile)
		{
			tile.update();

			if (tile.props.breakable && tile.props.collidable && tile.props.health <= 0)
			{
				const SDL_Point pos = tile.props.pos;
				tiles[pos.x + pos.y * size.x] = std::make_unique<DirtTile>(pos, *this, game);
				Mix_PlayChannel(1, game.audioc.getChunk(1), 0);
			}
		});

	em.update(delta);

	Entity *playerEntity = em.getEntity(player);
	Entity *chestEntity = em.getEntity(chest);

	if (playerEntity && chestEntity)
	{
		PlayerComponent *const pc = playerEntity->getComponent<PlayerComponent>();
		const TransformComponent *const ctfc = chestEntity->getComponent<TransformComponent>();
		const TransformComponent *const ptfc = playerEntity->getComponent<TransformComponent>();

		if (pc->preparation)
		{
			const std::uint32_t button = SDL_GetMouseState(&mouse.x, &mouse.y);

			const SDL_Point placement{static_cast<int>(mouse.x + camera.x) / Tile::Properties::size, static_cast<int>(mouse.y + camera.y) / Tile::Properties::size};
			const SDL_Point chestPos{static_cast<int>(ctfc->tf.x / Tile::Properties::size), static_cast<int>(ctfc->tf.y / Tile::Properties::size)};
			const SDL_Point playerPos{static_cast<int>(ptfc->tf.x / Tile::Properties::size), static_cast<int>(ptfc->tf.y / Tile::Properties::size)};

			buildBounds.w = game.getWindowSize().x - 160;
			buildBounds.h = game.getWindowSize().y - 160;

			if (isInside(placement) && tiles[placement.x + placement.y * size.x]->props.breakable && SDL_PointInRect(&mouse, &buildBounds))
			{
				outlineDstRect.x = placement.x * Tile::Properties::size - camera.x;
				outlineDstRect.y = placement.y * Tile::Properties::size - camera.y;
				canBuild = true;

				if (button == SDL_BUTTON_LMASK
					&& !(placement.x == chestPos.x && placement.y == chestPos.y)
					&& !((placement.x == playerPos.x || placement.x == playerPos.x + 1) && (placement.y == playerPos.y || placement.y == playerPos.y + 1)))
				{
					const auto &tile = tiles[placement.x + placement.y * size.x];

					if ((pc->tileSelected != tile->props.commonId && pc->points >= costs.at(pc->tileSelected)) || (pc->tileSelected == tile->props.commonId && tile->props.health != tile->props.maxHealth))
					{
						if (costs.find(tile->props.commonId) != costs.end())
							pc->points += costs.at(tile->props.commonId);

						switch (pc->tileSelected)
						{
						case 3:
							tiles[placement.x + placement.y * size.x] = std::make_unique<WoodTile>(placement, *this, game);
							break;
						case 6:
							tiles[placement.x + placement.y * size.x] = std::make_unique<StoneBrickTile>(placement, *this, game);
							break;
						case 7:
							tiles[placement.x + placement.y * size.x] = std::make_unique<BrickTile>(placement, *this, game);
							break;
						case 8:
							tiles[placement.x + placement.y * size.x] = std::make_unique<IronTile>(placement, *this, game);
							break;
						case 9:
							tiles[placement.x + placement.y * size.x] = std::make_unique<ObsidianTile>(placement, *this, game);
							break;
						case 10:
							tiles[placement.x + placement.y * size.x] = std::make_unique<MetalTile>(placement, *this, game);
							break;
						}

						pc->points -= costs.at(pc->tileSelected);
						Mix_PlayChannel(5, game.audioc.getChunk(5), 0);
						Mix_Volume(5, 80);
					}
					else if (pc->tileSelected != tile->props.commonId && pc->points < costs.at(pc->tileSelected) && !playedCantPlace)
					{
						Mix_PlayChannel(5, game.audioc.getChunk(8), 0);
						playedCantPlace = true;
					}
				}
				else if (button == SDL_BUTTON_RMASK)
				{
					auto &tile = tiles[placement.x + placement.y * size.x];

					if (costs.find(tile->props.commonId) != costs.end())
						pc->points += costs.at(tile->props.commonId);

					bool removed = false;

					switch (game.gameSave.level)
					{
					case 1:
						if (tile->props.commonId != 0)
						{
							tile = std::make_unique<GrassTile>(placement, *this, game);
							removed = true;
						}
						break;
					case 2:
						if (tile->props.commonId != 5)
						{
							tile = std::make_unique<SandTile>(placement, *this, game);
							removed = true;
						}
						break;
					case 3:
						if (tile->props.commonId != 4)
						{
							tile = std::make_unique<SnowTile>(placement, *this, game);
							removed = true;
						}
						break;
					case 4:
						if (tile->props.commonId != 0)
						{
							tile = std::make_unique<GrassTile>(placement, *this, game);
							removed = true;
						}
						break;
					case 5:
						if (tile->props.commonId != 0)
						{
							tile = std::make_unique<GrassTile>(placement, *this, game);
							removed = true;
						}
						break;
					case 6:
						if (tile->props.commonId != 4)
						{
							tile = std::make_unique<SnowTile>(placement, *this, game);
							removed = true;
						}
						break;
					case 7:
						if (tile->props.commonId != 0)
						{
							tile = std::make_unique<GrassTile>(placement, *this, game);
							removed = true;
						}
						break;
					case 8:
						if (tile->props.commonId != 5)
						{
							tile = std::make_unique<SandTile>(placement, *this, game);
							removed = true;
						}
						break;
					case 9:
						if (tile->props.commonId != 4)
						{
							tile = std::make_unique<SnowTile>(placement, *this, game);
							removed = true;
						}
						break;
					case 10:
						if (tile->props.commonId != 1)
						{
							tile = std::make_unique<DirtTile>(placement, *this, game);
							removed = true;
						}
						break;
					default:
						if (tile->props.commonId != 0)
						{
							tile = std::make_unique<GrassTile>(placement, *this, game);
							removed = true;
							LOG_ERROR(SDL_LOG_CATEGORY_APPLICATION, std::string{"Tile replacement not set for level " + std::to_string(game.gameSave.level) + "."}.c_str());
						}
						break;
					}

					if (removed)
					{
						Mix_PlayChannel(6, game.audioc.getChunk(6), 0);
						Mix_Volume(6, 80);
					}
				}
				else if (button != SDL_BUTTON_LMASK)
					playedCantPlace = false;
			}
			else
				canBuild = false;

			pc->prepareTimer -= 1 * delta;

			if (pc->prepareTimer <= 0.0f || wave == finalWave)
			{
				pc->preparation = false;
				pc->prepareTimer = pc->prepareTime;

				progress();
			}
		}
		else if (pc->em.getHostileCount() == 0)
			pc->preparation = true;
	}
	else
	{
		if (playerEntity)
			game.gsm.add(std::make_unique<GameOverMenuState>(game, playerEntity->getComponent<PlayerComponent>()->score));
	}
}


void World::render() const noexcept
{
	SDL_Renderer *const rdr = game.getRenderer();

	forAllVisibleTiles([rdr](Tile &tile)
		{
			tile.render(rdr);
		});

	//const SDL_Rect view{getView()};
	//for (int y = view.y; y <= view.h; ++y)
	//	for (int x = view.x; x <= view.w; ++x)
	//	{
	//		const int lval = light[x + y * size.x];
	//		if (lval <= 0 || lval > maxDarkness) continue;

	//		const SDL_Rect srcRect = {firstLightSrc.x, firstLightSrc.y + firstLightSrc.h * (lval - 1), firstLightSrc.w, firstLightSrc.h};
	//		const SDL_FRect dstRect{x * Tile::Properties::size - camera.x, y * Tile::Properties::size - camera.y, Tile::Properties::size, Tile::Properties::size};

	//		SDL_RenderCopyF(rdr, lightSheet, &srcRect, &dstRect);
	//	}

	if (canBuild)
	{
		const Entity *const playerEntity = em.getEntity(player);

		if (playerEntity)
		{
			const PlayerComponent *const pc = playerEntity->getComponent<PlayerComponent>();

			if (pc && pc->preparation)
				SDL_RenderCopyF(rdr, game.texc.get(2), &outlineSrcRect, &outlineDstRect);
		}
	}

	em.render(rdr);
}

//void World::emitLight(SDL_Point pos, int radius, int darkness) noexcept
//{
//	int r = darkness;
//	for (int d = this->darkness; d > darkness; --d)
//	{
//		createLightRadius(pos, radius + this->darkness - r, d);
//		++r;
//	}
//	createLightRadius(pos, radius, darkness);
//}

//void World::createLightRadius(SDL_Point pos, int radius, int darkness) noexcept
//{
//	std::function<void(SDL_Point pos, int darkness)> setLight = [&](SDL_Point pos, int darkness)
//	{
//		if (!isInside(pos)) return;
//
//		if (light[pos.x + pos.y * size.x] > darkness)
//			light[pos.x + pos.y * size.x] = darkness;
//	};
//
//	setLight(pos, darkness);
//
//	for (int i = 0; i < radius; ++i)
//	{
//		for (int x = pos.x; x > pos.x - (radius - i); --x)
//			setLight({x, pos.y - i}, darkness);
//		for (int x = pos.x; x < pos.x + (radius - i); ++x)
//			setLight({x, pos.y - i}, darkness);
//		for (int x = pos.x; x > pos.x - (radius - i); --x)
//			setLight({x, pos.y + i}, darkness);
//		for (int x = pos.x; x < pos.x + (radius - i); ++x)
//			setLight({x, pos.y + i}, darkness);
//	}
//}

std::unique_ptr<Tile> &World::getTile(SDL_Point pos) noexcept
{
	return tiles[pos.x + pos.y * size.x];
}

const std::unique_ptr<Tile> &World::getTile(SDL_Point pos) const noexcept
{
	return tiles[pos.x + pos.y * size.x];
}

bool World::isInside(SDL_Point pos) const noexcept
{
	return pos.x >= 0 && pos.x < size.x &&pos.y >= 0 && pos.y < size.y;
}

int World::getTileCost(std::uint32_t id) const noexcept
{
	if (costs.find(id) != costs.end())
		return costs.at(id);

	return 0;
}

void World::forAllVisibleTiles(std::function<void(Tile &tile)> func) const noexcept
{
	const SDL_Rect view{getView()};

	for (int y = view.y; y <= view.h; ++y)
		for (int x = view.x; x <= view.w; ++x)
			func(*tiles[x + y * size.x]);
}

//void World::forAllVisibleLight(std::function<void(int &light)> func) noexcept
//{
//	const SDL_Rect view{getView()};
//
//	for (int y = view.y; y <= view.h; ++y)
//		for (int x = view.x; x <= view.w; ++x)
//			func(light[x + y * size.x]);
//}

SDL_Rect World::getView() const noexcept
{
	SDL_Rect view{static_cast<int>(camera.x / Tile::Properties::size),
		static_cast<int>(camera.y / Tile::Properties::size),
		static_cast<int>((camera.x + game.getWindowSize().x) / Tile::Properties::size),
		static_cast<int>((camera.y + game.getWindowSize().y) / Tile::Properties::size)};

	if (view.x < 0) view.x = 0;
	if (view.w >= size.x) view.w = size.x - 1;
	if (view.y < 0) view.y = 0;
	if (view.h >= size.y) view.h = size.y - 1;

	return view;
}

void World::load(const std::string &path)
{
	SDL_Surface *surface = IMG_Load(path.c_str());
	if (!surface)
		throw std::runtime_error{SDL_GetError()};

	size.x = surface->w;
	size.y = surface->h;

	if (!tiles.empty()) tiles.clear();
	tiles.reserve(size.x * size.y);

	if (SDL_MUSTLOCK(surface))
		SDL_LockSurface(surface);

	for (int y = 0; y < size.y; ++y)
		for (int x = 0; x < size.x; ++x)
		{
			const std::uint8_t *pixel = static_cast<std::uint8_t *>(surface->pixels) + y * surface->pitch + x * surface->format->BytesPerPixel;
			std::uint8_t r, g, b;
			int hex;

			if (surface->format->BytesPerPixel == 4)
			{
				std::uint8_t a;
				SDL_GetRGBA(static_cast<std::uint32_t>(*pixel), surface->format, &r, &g, &b, &a);
				hex = ((r & 0xFF) << 24) + ((g & 0xFF) << 16) + ((b & 0xFF) << 8) + (a & 0xFF);
			}
			else
			{
				SDL_GetRGB(static_cast<std::uint32_t>(*pixel), surface->format, &r, &g, &b);
				hex = ((r & 0xFF) << 16) + ((g & 0xFF) << 8) + (b & 0xFF);
			}

			switch (hex)
			{
			case 0x108329:
				tiles.emplace_back(std::make_unique<GrassTile>(SDL_Point{x, y}, *this, game));
				break;

			case 0x512923:
				tiles.emplace_back(std::make_unique<DirtTile>(SDL_Point{x, y}, *this, game));
				break;

			case 0x434343:
				tiles.emplace_back(std::make_unique<StoneTile>(SDL_Point{x, y}, *this, game));
				break;

			case 0x716039:
				tiles.emplace_back(std::make_unique<WoodTile>(SDL_Point{x, y}, *this, game));
				break;

			case 0x979797:
				tiles.emplace_back(std::make_unique<SnowTile>(SDL_Point{x, y}, *this, game));
				break;

			case 0x7D7838:
				tiles.emplace_back(std::make_unique<SandTile>(SDL_Point{x, y}, *this, game));
				break;

			case 0x555555:
				tiles.emplace_back(std::make_unique<StoneBrickTile>(SDL_Point{x, y}, *this, game));
				break;

			case 0x081C8E:
				tiles.emplace_back(std::make_unique<WaterTile>(SDL_Point{x, y}, *this, game));
				break;

			default:
				LOG_ERROR(SDL_LOG_CATEGORY_APPLICATION, "Unknown tile hex!");
				break;
			}

			//light.emplace_back(darkness);
		}

	if (SDL_MUSTLOCK(surface))
		SDL_UnlockSurface(surface);

	SDL_FreeSurface(surface);

	em.renderable.clear();
	em.collidables.clear();
	em.entities.clear();

	const SDL_FPoint start{((size.x - 1) * Tile::Properties::size - Tile::Properties::size) / 2 + 4.0f, ((size.y - 1) * Tile::Properties::size - Tile::Properties::size) / 2 + 4.0f};
	chest = createChest(em, start, *this, game);
	player = createPlayer(em, {start.x, start.y + 100}, DirectionalAnimationComponent::Direction::down, *this, game);
}

void World::progress() noexcept
{
	if (wave == finalWave)
	{
		if (game.gameSave.level == finalLevel)
		{
			game.gsm.add(std::make_unique<WinMenuState>(game, em.getEntity(getPlayer())->getComponent<PlayerComponent>()->score));
			return;
		}
		else
			game.gsm.add(std::make_unique<LevelPassMenuState>(game, em.getEntity(getPlayer())->getComponent<PlayerComponent>()->score));

		++game.gameSave.level;
		wave = 0;

		load("Resources/Levels/Level" + std::to_string(game.gameSave.level) + ".png");
		game.gameSave.save();
	}
	else
		++wave;

	const int enemyCount = 6 + (3 * wave);
	std::vector<SDL_Point> positions;
	positions.reserve(enemyCount);

	std::function<SDL_Point()> getRandomSpawn = [size = size]()
	{
		int side = getRandomInt(0, 4);

		if (side == 0)
			return SDL_Point{getRandomInt(0, size.x - 1), getRandomInt(0, 5)};
		else if (side == 1)
			return SDL_Point{getRandomInt(0, size.x - 1), getRandomInt(size.y - 6, size.y - 1)};
		else if (side == 2)
			return SDL_Point{getRandomInt(0, 5), getRandomInt(0, size.y - 1)};
		else
			return SDL_Point{getRandomInt(size.x - 6, size.x - 1), getRandomInt(0, size.y - 1)};
	};

	std::function<bool(SDL_Point)> isSpawnTaken = [&positions](SDL_Point spawn)
	{
		for (SDL_Point pos : positions)
		{
			if (spawn.x == pos.x && spawn.y == pos.y)
				return true;
		}

		return false;
	};

	for (int i = 0; i < enemyCount; ++i)
	{
		SDL_Point spawnCoords;

		do
			spawnCoords = getRandomSpawn();
		while (tiles[spawnCoords.x + spawnCoords.y * size.x]->props.collidable || isSpawnTaken(spawnCoords));

		positions.emplace_back(spawnCoords);
		spawnRandomHostile({static_cast<float>(spawnCoords.x * Tile::Properties::size), static_cast<float>(spawnCoords.y * Tile::Properties::size)});
	}
}

void World::spawnRandomHostile(SDL_FPoint pos) noexcept
{
	const int type = getRandomInt(0, 7);

	switch (type)
	{
	case 0:
		createSlime(em, pos, SingleAxisAnimationComponent::Direction::neg, *this, game);
		break;
	case 1:
		createArcher(em, pos, DirectionalAnimationComponent::Direction::up, *this, game);
		break;
	case 2:
		createDarkLord(em, pos, DirectionalAnimationComponent::Direction::up, *this, game, static_cast<Projectile>(getRandomInt(0, 3)));
		break;
	case 3:
		createGoblin(em, pos, DirectionalAnimationComponent::Direction::up, *this, game);
		break;
	case 4:
		createIceDemon(em, pos, DirectionalAnimationComponent::Direction::up, *this, game);
		break;
	case 5:
		createZombie(em, pos, DirectionalAnimationComponent::Direction::up, *this, game);
		break;
	case 6:
		createKnight(em, pos, DirectionalAnimationComponent::Direction::up, *this, game);
		break;
	case 7:
		createLootLord(em, pos, DirectionalAnimationComponent::Direction::up, *this, game);
		break;
	}
}
