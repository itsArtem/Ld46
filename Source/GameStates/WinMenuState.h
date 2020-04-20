#pragma once

#include "GameState.h"
#include "../Graphics/Gui/GuiCanvas.h"
#include "../Graphics/Text.h"

class Game;
class World;

class WinMenuState final : public GameState
{
public:
	WinMenuState(Game &game, int finalScore);

	void update() noexcept override;
	void render() const noexcept override;

private:
	Game &game;
	const World &world;

	Text title, info;
	GuiCanvas canvas;
};