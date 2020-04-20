#pragma once

#include "GameState.h"
#include "../Graphics/Gui/GuiCanvas.h"
#include "../Graphics/Text.h"

class Game;

class LevelPassMenuState final : public GameState
{
public:
	LevelPassMenuState(Game &game, int score);

	void update() noexcept override;
	void render() const noexcept override;

private:
	Game &game;
	Text title, info;
	GuiCanvas canvas;
};