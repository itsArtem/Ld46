#include "GameplayState.h"
#include "../Game.h"
#include "../Graphics/Gui/TextureButton.h"
#include "ShopState.h"
#include "../World/Ecs/Entity.h"
#include "../World/Ecs/EntityManager.h"
#include "../World/Ecs/Component.h"
#include "../GameStates/PauseMenuState.h"

#include <SDL_keyboard.h>

GameplayState::GameplayState(Game &game) noexcept
	: game{game},
	world{game},
	hudm{game, world}
{
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{game.getWindowSize().x - 86, game.getWindowSize().y - 78, 64, 64}, game, TextureButton::Visuals{game.texc.get(2), SDL_Rect{32, 16, 16, 16}, SDL_Rect{48, 16, 16, 16}, SDL_Rect{64, 16, 16, 16}}));
}

void GameplayState::update() noexcept
{
	const Entity *const player = world.em.getEntity(world.getPlayer());

	if (player && player->getComponent<PlayerComponent>()->preparation)
	{
		canvas.get(0).tf.x = game.getWindowSize().x - 86;
		canvas.get(0).tf.y = game.getWindowSize().y - 78;

		if (canvas.get(0).isReleased())
		{
			game.gsm.add(std::make_unique<ShopState>(game, game.gsm, world));
			game.gsm.renderPrevious(true);
		}

		canvas.update();
	}

	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_ESCAPE])
		game.gsm.add(std::make_unique<PauseMenuState>(game));

	world.update();

	hudm.update();

	tipTimer -= 1.0f * game.getDelta();

	if (tipTimer < 0.0f)
	{
		if (!showedMove)
		{
			hudm.msgout.showMessage("Move around with WASD.", 4.0f, {8.0f, game.getWindowSize().y - 190.0f});
			tipTimer = 5.0f;
			showedMove = true;
		}
		else if (!showedShop)
		{
			hudm.msgout.showMessage("Select blocks by clicking the shop icon.", 4.0f, {8.0f, game.getWindowSize().y - 190.0f});
			tipTimer = 5.0f;
			showedShop = true;
		}
		else if (!showedBuild)
		{
			hudm.msgout.showMessage("Buy blocks by left clicking.", 4.0f, {8.0f, game.getWindowSize().y - 190.0f});
			tipTimer = 5.0f;
			showedBuild = true;
		}
		else if (!showedRemove)
		{
			hudm.msgout.showMessage("Sell blocks by right clicking.", 4.0f, {8.0f, game.getWindowSize().y - 190.0f});
			tipTimer = 5.0f;
			showedRemove = true;
		}
		else if (!showedPoints)
		{
			hudm.msgout.showMessage("You gain points by damaging hostiles.", 4.0f, {8.0f, game.getWindowSize().y - 190.0f});
			tipTimer = 5.0f;
			showedPoints = true;
		}
		else if (!showedFinal)
		{
			hudm.msgout.showMessage("Good luck!", 4.0f, {8.0f, game.getWindowSize().y - 190.0f});
			tipTimer = 5.0f;
			showedFinal = true;
		}
	}
}

void GameplayState::render() const noexcept
{
	world.render();
	hudm.render();

	const Entity *const player = world.em.getEntity(world.getPlayer());

	if (player && player->getComponent<PlayerComponent>()->preparation)
		canvas.render(game.getRenderer());
}