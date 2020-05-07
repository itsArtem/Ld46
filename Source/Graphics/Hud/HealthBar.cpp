#include "HealthBar.h"
#include "../../World/Ecs/Component.h"
#include "../../World/World.h"

HealthBar::HealthBar(World &world, const Game &game) noexcept
	: world{world},
	game{game}
{
}

void HealthBar::update() noexcept
{
	if (world.em.getEntity(world.getPlayer()))
	{
		const HealthComponent *const hc = world.em.getEntity(world.getPlayer())->getComponent<HealthComponent>();
		const float size = (hc->health / hc->maxHealth) * (barDst.w - 22);
		healthMetre.w = static_cast<int>(size);
	}

	if (world.em.getEntity(world.getChest()))
	{
		const HealthComponent *const chc = world.em.getEntity(world.getChest())->getComponent<HealthComponent>();
		const float chestSize = (chc->health / chc->maxHealth) * (chestBarDst.w - 22);

		chestIconDst.x = game.getWindowSize().x - 52;
		chestBarDst.x = chestIconDst.x - chestBarDst.w + 14;
		chestHealthMetre.x = (chestBarDst.x + 11);
		chestHealthMetre.w = static_cast<int>(chestSize);
	}
}

void HealthBar::render() const noexcept
{
	SDL_Renderer *const rdr = game.getRenderer();

	SDL_RenderCopy(rdr, ss, &emptyBarSrc, &barDst);
	SDL_SetRenderDrawColor(rdr, 100, 100, 100, 255);
	SDL_RenderFillRect(rdr, &healthMetre);
	SDL_RenderCopy(rdr, ss, &barSrc, &barDst);
	SDL_RenderCopy(rdr, ss, &iconSrc, &iconDst);

	SDL_RenderCopy(rdr, ss, &emptyBarSrc, &chestBarDst);
	SDL_SetRenderDrawColor(rdr, 100, 100, 100, 255);
	SDL_RenderFillRect(rdr, &chestHealthMetre);
	SDL_RenderCopy(rdr, ss, &barSrc, &chestBarDst);
	SDL_RenderCopy(rdr, ss, &chestIconSrc, &chestIconDst);
}
