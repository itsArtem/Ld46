#pragma once

#include "GameState.h"
#include "../Game.h"
#include "../Graphics/Gui/GuiCanvas.h"
#include "../Graphics/Text.h"

class MenuManager;
class GameStateManager;
class World;

class ShopState final : public GameState
{
public:
	ShopState(const Game &game, GameStateManager &gsm, World &world) noexcept;

	void update() noexcept override;
	void render() const noexcept override;

private:
	const Game &game;
	GameStateManager &gsm;
	World &world;

	SDL_Texture *const ss = game.texc.get(2);
	SDL_Rect tf{32, 32, game.getWindowSize().x - 96, game.getWindowSize().x - 96};
	const SDL_Rect cornerSrcRect{0, 0, 16, 16};
	const SDL_Rect edgeSrcRect{16, 0, 16, 16};
	const SDL_Rect centreSrcRect{32, 0, 16, 16};
	const int cellSize = 32;

	GuiCanvas canvas;
	Text title1, title2, title3, title4, title5;
	Text desc1, desc2, desc3, desc4, desc5;
	Text cost1, cost2, cost3, cost4, cost5;

	void exit() const noexcept;
};