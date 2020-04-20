#include "HudManager.h"

HudManager::HudManager(const Game &game, World &world) noexcept
	: msgout{game},
	healthBar{world, game},
	info{game, world}
{
}

void HudManager::update() noexcept
{
	msgout.update();
	healthBar.update();
	info.update();
}

void HudManager::render() const noexcept
{
	msgout.render();
	healthBar.render();
	info.render();
}
