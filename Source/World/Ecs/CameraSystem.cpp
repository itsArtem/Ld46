#include "CameraSystem.h"
#include "Entity.h"
#include "../Tile/Tile.h"
#include "Component.h"
#include "../../Game.h"
#include "../World.h"

#include <SDL_rect.h>

void CameraSystem::update(Entity &e) const noexcept
{
	CameraFocusComponent *const cfc = e.getComponent<CameraFocusComponent>();
	if (!cfc) return;

	TransformComponent *const tfc = e.getComponent<TransformComponent>();

	const SDL_Point winSize = cfc->game.getWindowSize();

	cfc->world.camera.x = tfc->tf.x + tfc->tf.w / 2 - winSize.x / 2;
	cfc->world.camera.y = tfc->tf.y + tfc->tf.h / 2 - winSize.y / 2;

	if (cfc->world.camera.x < 0.0f)
		cfc->world.camera.x = 0.0f;
	else if (cfc->world.camera.x + winSize.x > cfc->world.size.x * Tile::Properties::size)
		cfc->world.camera.x = static_cast<float>(cfc->world.size.x * Tile::Properties::size - winSize.x);

	if (cfc->world.camera.y < 0.0f)
		cfc->world.camera.y = 0.0f;
	else if (cfc->world.camera.y + winSize.y > cfc->world.size.y * Tile::Properties::size)
		cfc->world.camera.y = static_cast<float>(cfc->world.size.y * Tile::Properties::size - winSize.y);
}
