#pragma once

#include "GameState.h"
#include "../Graphics/Gui/GuiCanvas.h"
#include "../Graphics/Text.h"

class Game;
class World;

class GameOverMenuState final : public GameState
{
public:
	GameOverMenuState(Game &game, int finalScore);

	void update() noexcept override;
	void render() const noexcept override;

private:
	Game &game;

	Text title, info;
	GuiCanvas canvas;
};