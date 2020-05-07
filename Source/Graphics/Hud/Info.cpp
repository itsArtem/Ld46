#include "Info.h"
#include "../../World/World.h"
#include "../../World/Ecs/Entity.h"
#include "../../World/Ecs/Component.h"

#include <string>

Info::Info(const Game &game, const World &world) noexcept
	: game{game},
	world{world}
{
	prep.dstRect.x = 24.0f;
	prep.dstRect.y = game.getWindowSize().y - 108.0f;

	points.dstRect.x = game.getWindowSize().x - 210.0f;
	points.dstRect.y = game.getWindowSize().y - 32.0f;

	wave.dstRect.x = 24.0f;
	wave.dstRect.y = game.getWindowSize().y - 76.0f;

	level.dstRect.x = 24.0f;
	level.dstRect.y = game.getWindowSize().y - 44.0f;
}

void Info::update() noexcept
{
	const Entity *player = world.em.getEntity(world.getPlayer());

	prep.dstRect.y = game.getWindowSize().y - 108.0f;
	wave.dstRect.y = game.getWindowSize().y - 76.0f;
	level.dstRect.y = game.getWindowSize().y - 44.0f;

	if (player)
	{
		const PlayerComponent *const pc = player->getComponent<PlayerComponent>();

		if (pc->preparation)
		{
			points.dstRect.x = game.getWindowSize().x - 248.0f;
			points.dstRect.y = game.getWindowSize().y - 44.0f;

			prep.setText(game.fontc.get(0), "Prepare! " + std::to_string(static_cast<int>(pc->prepareTimer)) + "s left", {255, 255, 255, 255}, true, game.getRenderer());
			points.setText(game.fontc.get(0), std::to_string(static_cast<int>(pc->points)) + " points", {255, 255, 255, 255}, true, game.getRenderer());
		}
		else
			prep.setText(game.fontc.get(0), "Defend! " + std::to_string(world.em.getHostileCount()) + " Remaining", {255, 255, 255, 255}, true, game.getRenderer());
	}

	wave.setText(game.fontc.get(0), "Wave " + std::to_string(world.getWave()) + "/" + std::to_string(world.finalWave), {255, 255, 255, 255}, true, game.getRenderer());
	level.setText(game.fontc.get(0), "Level " + std::to_string(world.getLevel()), {255, 255, 255, 255}, true, game.getRenderer());
}

void Info::render() const noexcept
{
	SDL_Renderer *const rdr = game.getRenderer();

	const SDL_Rect leftDstRect{4, game.getWindowSize().y - 124, 296, 120};
	SDL_RenderCopy(rdr, bg, &lbgSrcRect, &leftDstRect);

	const Entity *player = world.em.getEntity(world.getPlayer());

	if (player)
	{
		const PlayerComponent *const pc = player->getComponent<PlayerComponent>();

		if (pc && pc->preparation)
		{
			const SDL_Rect rightDstRect{game.getWindowSize().x - 300, game.getWindowSize().y - 100, 296, 96};
			SDL_RenderCopy(rdr, bg, &rbgSrcRect, &rightDstRect);

			points.render(rdr, Text::Shadow::right);
		}
	}

	prep.render(rdr, Text::Shadow::right);
	wave.render(rdr, Text::Shadow::right);
	level.render(rdr, Text::Shadow::right);
}
