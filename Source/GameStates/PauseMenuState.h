#pragma once

#include "GameState.h"
#include "../Graphics/Gui/GuiCanvas.h"
#include "../Graphics/Text.h"

class Game;

class PauseMenuState final : public GameState
{
public:
	PauseMenuState(Game &game);

	void update() noexcept override;
	void render() const noexcept override;

private:
	Game &game;
	Text title, info;
	GuiCanvas canvas;
};