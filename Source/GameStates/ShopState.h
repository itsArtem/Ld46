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
	SDL_Rect tf{game.getWindowSize().x / 2 - tf.w / 2 - 16, 32, 876, 584};
	const SDL_Rect cornerSrcRect{0, 0, 16, 16};
	const SDL_Rect edgeSrcRect{16, 0, 16, 16};
	const SDL_Rect centreSrcRect{32, 0, 16, 16};
	const SDL_Rect barSrcRect{0, 112, 80, 8};
	const int cellSize = 32;

	GuiCanvas canvas;
	Text title0, title1, title2, title3, title4, title5, title6;
	Text desc1, desc2, desc3, desc4, desc5, desc6;
	Text cost1, cost2, cost3, cost4, cost5, cost6;

	float blue = 0.0f;
	float inc = 600.0f;

	void exit() const noexcept;
};